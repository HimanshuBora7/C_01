#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {

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

    char buffer[100];
    int n = recv(fd, buffer, sizeof(buffer), 0);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Received: %s", buffer);
    }

    close(fd);
    return 0;
}