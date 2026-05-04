#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{

    // making endpoint or socket
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("failed");
        return 1;
    }

    // binding the socket to address so endpoint can have an address
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/mysocket");

    unlink("/tmp/mysocket");
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind failed");
        return 1;
    }

    // listen part

    if (listen(fd, 5) == -1)
    {
        perror("listen failed");
        return 1;
    }
    printf("listening...\n");
    int client_fd = accept(fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("accept failed");
        return 1;
    }

    char buffer[100];
    fd_set readfds;

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(client_fd, &readfds);

        int maxfd = client_fd;
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            perror("select failed");
            break;
        }
        if (FD_ISSET(client_fd, &readfds))
        {
            int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

            if (n <= 0)
            {
                printf("Client disconnected\n");
                break;
            }

            buffer[n] = '\0';
            printf("Client: %s", buffer);
        }
        if (FD_ISSET(0, &readfds))
        {
            printf("You: ");
            fflush(stdout);

            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                break;

            if (strncmp(buffer, "exit\n", 5) == 0)
                break;

            if (send(client_fd, buffer, strlen(buffer), 0) == -1)
            {
                perror("send failed");
                break;
            }
        }
    }
    close(client_fd);
    close(fd);
    return 0;
}