#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define BACKLOG 10
#define MYPORT "3493"

int main(){
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, newfd;
    char *msg;
    pid_t p = fork();

    if(p == 0){
        // listening to client
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, MYPORT, &hints, &res);

        if( (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
            printf("Parent/Send: Unable to create socket\n");
        }
        if( (connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1 ){
            printf("Parent/Sender: Failed to connect\n");
        }

        msg = (char *)malloc(sizeof(char)*100);

        while(true){
            // *msg = "Hello World";
            printf("Enter: ");
            scanf("%s", msg);
            int length = strlen(msg);
            int sent = send(sockfd, msg, length, 0);
            printf("Parent/Sender: Sent %i\n", sent);
            sleep(1);
        }
        free(msg);
    }
    else{
        // send to client
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, MYPORT, &hints, &res);

        if( (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
            printf("Child/Receive: Unable to create socket\n");
        };
        
        if( (bind(sockfd, res->ai_addr, res->ai_addrlen)) == -1 ){
            printf("Child/Receive: Unable to bind to socket\n");
        };

        listen(sockfd, BACKLOG);

        addr_size = sizeof their_addr;
        newfd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
        
        msg = (char *)malloc(sizeof(char)*100);
        int length = strlen(msg);

        while(true){
            int hello = recv(newfd, msg, 99, 0);
            printf("Child/Receiver: Received %i\n", hello);
            printf("%s\n", msg);
            sleep(1);
        }
        free(msg);
    }
    
    return 0;
}