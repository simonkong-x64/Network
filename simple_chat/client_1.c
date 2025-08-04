#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define SEND_PORT "3491"
#define RCV_PORT "3490"
#define BACKLOG 10
#define MYPORT "3492"

int main(){
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, newfd;
    char msg[100];
    pid_t p = fork();

    if(p == 0){
        // listening to client
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, MYPORT, &hints, &res);

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        newfd = connect(sockfd, res->ai_addr, res->ai_addrlen);
        while(true){
            printf("Enter: \n");
            scanf("%s", msg);
            send(newfd, msg, sizeof msg, 0);
        }
    }
    else{
        // send to client
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, MYPORT, &hints, &res);

        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        bind(sockfd, res->ai_addr, res->ai_addrlen);
        listen(sockfd, BACKLOG);

        addr_size = sizeof their_addr;
        newfd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
        recv(newfd, );
    }
    

    
    return 0;
}