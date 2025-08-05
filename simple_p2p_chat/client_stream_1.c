#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>

#define TO_PORT 4020
#define MY_PORT 4021
#define BACKLOG 10

int main(){
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in their_address;
    char* msg;
    int rcv, sock_fd, new_fd, pid;
   
    
    if( (pid = fork()) == 0){
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if(getaddressinfo(NULL, MY_PORT, &hints, &servinfo) != 0){
            printf("failed to get address info\n");
        }

        for(p = servinfo; p != NULL; p = p->ai_next){
            if( (sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
                printf("failed to create socket")
                continue;
            }

            if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, true, sizeof(int)) == -1){
                perror("setsockopt\n");
                exit(1);
            }

            if( bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1){
                close(sock_fd);
                perror("server:bind\n");
                continue;
            }
            break;

            freeaddrinfo(servinfo);

            if(p == NULL){
                printf("failed to bind\n");
                exit(1);
            }

            if( listen(sock_fd, BACKLOG) == -1 ){
                printf("Failed to listen");
            }

            if( (new_fd = accept(sock_fd, ))
            while(true){
            }
        }


    }
    else{

    }


    if( (rcv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0 ){
        printf("Failed to get address info\n");
    }

    if( () )
    
    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if( listen(sock_fd, BACKLOG) != 0){
        printf("Failed to listen to socket\n");
    }

    printf("server: waiting for connections...\n");
    
    return 0;
}
