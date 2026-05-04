// client side of the program

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    // parameters passed, protocol we want, stream we want (tcp/udp), the IP layer we want to work beneath transport layer
    // return fd (socker file descriptor)
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    // we can use this fd to connect some remote socket
    // so we use connect function it takes fd , pointer to the address and length of address

    // generic structure we pass as address

    struct sockaddr_in address;
    char *ip = "127.0.0.1";

    // setting the properties of address

    // we can't directly assign the port address
    //  this htons converts the port address to big endian format as tcp n udp expects port address in form of big endian

    address.sin_port = htons(2000); // this port should be the port of server that is listening on the other side so there must be a process listening on same port on the address we gonna specify
    address.sin_family = AF_INET;

    // ip address passsed here need to be converted into unsigned byte so to make this work we have a function if converts the ip into unsigned byte and put it in address property
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);

    int result = connect(socketFD, &address, sizeof(address));

    if (result == 0)
    {
        printf("connection established\n");
    }
    memset(&address, 0, sizeof(address));
    char *line = NULL;

    size_t linesize = 0;
    printf("type a text to send or (exit)\n");
    while (true)
    {
        ssize_t charCount = getline(&line, &linesize, stdin);

        if (charCount > 0)
        {
            if (strcmp(line, "exit\n") == 0)
            {
                break;
            }
            ssize_t amountwasSent = send(socketFD, line, charCount, 0);
        }
    }
    close(socketFD);
    return 0;
}