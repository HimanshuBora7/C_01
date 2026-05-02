#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
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
    printf("%d",fd);
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
}send(client_fd, "hello\n", 6, 0);
close(client_fd);
    return 0;
}