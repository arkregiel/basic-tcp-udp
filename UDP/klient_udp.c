#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MSG_SIZE 256
#define PORT 8080
#define ADDR "127.0.0.1"

int main()
{
    int sockfd;
    struct sockaddr_in addr;
    char msg[MSG_SIZE];
    socklen_t addrlen;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ADDR);
    addr.sin_port = htons(PORT);

    printf("[Klient] Tworzenie gniazda... ");
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("BŁĄÐ\n");
        perror("socket");
        exit(-1);
    }
    printf("OK\n");

    printf("Napisz wiadomość do serwera:\n> ");

    fgets(msg, MSG_SIZE, stdin);
    msg[strlen(msg) - 1] = '\0';

    printf("[Klient] Wysyłanie wiadomości... ");
    addrlen = sizeof(addr);
    if(sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&addr, addrlen) < 0)
    {
        printf("BŁĄÐ\n");
        perror("sendto");
        exit(-1);
    }
    printf("OK\n");

    printf("[Klient] Kończenie\n");

    close(sockfd);

    return 0;
}