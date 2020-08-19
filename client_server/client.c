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
    //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);

    ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1) die(__LINE__, "connect error");

    fprintf(stdout, "connected to the server\n");

    char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
    size_t size = BUFF_SIZE * sizeof(char);
    int end = 0;

    while(1)
    {
        fprintf(stdout, "client> ");
        //fscanf(stdin, "%s", buff);
        util_getline(buff, size, stdin);
        
        if(strncmp(buff, "exit", 4) == 0)
        {
            end = 1;
        }

        send(sockfd, buff, size, 0);

        memset(buff, 0, size);
        recv(sockfd, buff, size, 0);

        fprintf(stdout, "received: %s\n", buff);

        if(end) break;
    }

    free(buff);

    close(sockfd);

    return 0;
}