#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include<sys/select.h>

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
    fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        // tell it what to watch 
        FD_SET(0, &readfds);   // stdin
        FD_SET(fd, &readfds);  // socket

        int maxfd = fd;
        //wait
        if(select(maxfd+1, &readfds,NULL,NULL,NULL) == -1){
            perror("select failed");
            break;
        }
         if (FD_ISSET(0, &readfds)) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                break;

            if (strncmp(buffer, "exit\n", 5) == 0)
                break;

            send(fd, buffer, strlen(buffer), 0);
        }
          if (FD_ISSET(fd, &readfds)) {
            int n = recv(fd, buffer, sizeof(buffer) - 1, 0);

            if (n <= 0) {
                printf("Server disconnected\n");
                break;
            }

            buffer[n] = '\0';
            printf("Server: %s", buffer);
        }
    }

    close(fd);
    return 0;
}