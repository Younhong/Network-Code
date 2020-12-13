#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock;
    int client_sock;
    char message[BUFSIZE];
    int str_len;
    int num = 0;
    char filename[10];
    char AorB[6];

    FILE *rf, *f;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

    int client_addr_size;

    if (argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    client_addr_size = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, (socklen_t *)&client_addr_size);
    if (client_sock == -1) {
        error_handling("accept() error");
    }

    str_len = recv(client_sock, filename, BUFSIZE, 0);
    filename[str_len] = 0;
    send(client_sock, filename, strlen(filename) + 1, 0);

    str_len = recv(client_sock, AorB, BUFSIZE, 0);

    if (strcmp(AorB, "ascii") == 0) {
        rf = fopen(filename, "w");
    } else if (strcmp(AorB, "binary") == 0) {
        rf = fopen(filename, "wb");
    } else {
        error_handling("Invalid Option");
    }

    while ((str_len = recv(client_sock, message, BUFSIZE, 0)) != 0){
        message[str_len] = 0;
        send(client_sock, message, strlen(message)+1, 0);
        fputs(message, f);
    }

    close(client_sock);
	return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}