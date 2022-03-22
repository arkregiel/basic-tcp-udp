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

int sockfd;

void end(int sig)
{
    printf("\n[ ==> SIGINT ]\n[Serwer] Kończenie działania\n");
    close(sockfd);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, end);
    int newsockfd, n;
    //char msg[MSG_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t clilen;
    int reuse = 1;

    printf("[Serwer] Tworzenie gniazda... ");

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("BŁĄÐ\n");
        perror("socket");
        exit(-1);
    }
    printf("OK\n");

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    printf("[Serwer] Wiązanie gniazda... ");
    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("BŁĄD\n");
        perror("bind");
        exit(-1);
    }
    printf("OK\n");

    printf("[Serwer] Rozpoczynanie nasłuchiwania na porcie %d...", PORT);

    if(listen(sockfd, 5) < 0)
    {
        printf("BŁĄD\n");
        perror("listen");
        exit(-1);
    }
    printf("OK\n");

    while(1)
    {
        char msg[MSG_SIZE] = {'\0'};
        clilen = sizeof(client_addr);
        if((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen)) < 0)
        {
            perror("accept");
            exit(-1);
        }
        printf("[+] Nowe połączenie od [%s:%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if((n = read(newsockfd, msg, MSG_SIZE)) < 0)
        {
            perror("read");
            exit(-1);
        }

        printf("[Klient:] %s\n", msg);

        printf("[-] Zamykanie połączenia\n");
        close(newsockfd);
    }

    return 0;
}