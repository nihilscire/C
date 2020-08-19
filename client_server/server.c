#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include "util.h"

#define PORT 8080 
#define BUFF_SIZE 500

int main()
{
    int ret = 0;
    int connfd = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);    

    if(sockfd == -1)
    {
        die(__LINE__, "socket creation error");
    }

    int optval = 1;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval));
    if(ret == -1) die(__LINE__, "set socket option error");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) die(__LINE__, "bind error");

    ret = listen(sockfd, 10);
    if(ret == -1) die(__LINE__, "listen error");

    struct sockaddr_in client;
    socklen_t client_size;

    fprintf(stdout, "waiting for connections...\n");

    connfd = accept(sockfd, (struct sockaddr*)&client, &client_size);
    //if(connfd == -1) die(__LINE__, "accept error");

    printf("server: got connection from %s port %d\n",
            inet_ntoa(client.sin_addr),
            ntohs(client.sin_port));


    fprintf(stdout, "connection established\n");

    size_t size = BUFF_SIZE * sizeof(char);
    char* buff = (char*)malloc(size);

    while(1)
    {
        memset(buff, 0, size);

        recv(connfd, buff, size, 0);

        fprintf(stdout, "received: %s\n", buff);

        if(strncmp(buff, "exit", 4) == 0)
        {
            memset(buff, 0, size);
            strncpy(buff, "server exit! bye!\n", size);
            send(connfd, buff, size, 0);
            fprintf(stdout, "server exit! bye!\n");

            break;
        }

        send(connfd, buff, size, 0);
    }

    free(buff);

    close(sockfd);

    return 0;
}