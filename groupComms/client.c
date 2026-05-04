// client side of the program

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>
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
    inet_pton(AF_INET, ip, &address.sin_addr);

    int result = connect(socketFD, (struct sockaddr *)&address, sizeof(address));

    if (result == 0)
    {
        printf("connection established\n");
    }
    memset(&address, 0, sizeof(address));
    char *line = NULL;

    size_t linesize = 0;
    printf("type a text to send or (exit)\n");

    fd_set readfds;
    char buffer[1024];

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);        // stdin
        FD_SET(socketFD, &readfds); // socket

        int maxfd = socketFD;

        select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(socketFD, &readfds))
        {
            int bytes = recv(socketFD, buffer, sizeof(buffer) - 1, 0);

            if (bytes <= 0)
            {
                printf("Disconnected from server\n");
                break;
            }

            buffer[bytes] = '\0';
            printf("%s", buffer);
        }
        if (FD_ISSET(0, &readfds))
        {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                break;

            if (strcmp(buffer, "exit\n") == 0)
                break;

            send(socketFD, buffer, strlen(buffer), 0);
        }
    }
    close(socketFD);
    return 0;
}