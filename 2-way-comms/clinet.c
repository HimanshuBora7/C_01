#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(){

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/mysocket");

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect failed");
        return 1;
    }

    char buffer[50];

    while (1) {

        printf("You: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        if (send(fd, buffer, strlen(buffer), 0) == -1) {
            perror("send failed");
            break;
        }

        int n = recv(fd, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }

        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }

    close(fd);
    return 0;
}