#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
int main(){
    
    //making endpoint or socket
    int fd = socket(AF_UNIX,SOCK_STREAM,0);
    if(fd == -1){
        printf("failed");
        return 1; 
    }

    //binding the socket to address so endpoint can have an address 
    struct sockaddr_un addr;
    memset(&addr,0,sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/mysocket");
   
    unlink("/tmp/mysocket");
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        return 1;
    }
 
    //listen part

    
   if (listen(fd, 5) == -1) {
    perror("listen failed");
    return 1;
}
printf("listening...\n");
   int client_fd = accept(fd, NULL, NULL);
if (client_fd == -1) {
    perror("accept failed");
    return 1;
}

char buffer[100];
while(1)
{
    int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) {
            printf("client disconnected\n");
            break;
        }

        buffer[n] = '\0';
        printf("client: %s", buffer);

    printf("You: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send failed");
            break;
        }
    }
    close(client_fd);
close(fd);
    return 0;
}