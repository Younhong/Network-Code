#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;
    int str_len;
    char message[BUFSIZE];
    struct sockaddr_in serv_addr;

    if (argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    char MSG[3][10] = {"Good", "Evening", "Everybody"};

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }

    for (int i = 0; i < 3; i++) {
        send(sock, MSG[i], strlen(MSG[i]), 0);

        str_len = recv(sock, message, BUFSIZE-1, 0);
        message[str_len] = 0;
        printf("서버로부터 수신된 %d차 메시지: %s\n", i, message);
    }

    close(sock);
	return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}