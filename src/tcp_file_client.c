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
    char filename[10];
    char AorB[6];
    FILE *f, *rf;
    int str_len;
    int sock;
    char message[BUFSIZE];

    printf("Enter the name of text file to read: ");
    scanf("%s", filename);
    printf("A for ascii, B for binary: ");
    scanf("%s", AorB);

    if (strcmp(AorB, "ascii") == 0){
        f = fopen(filename, "r");
        if (f == NULL){
            printf("Text file does not exist");
            exit(1);
        }
    } else if (strcmp(AorB, "binary") == 0) {
        f = fopen(filename, "rb");
        if (f == NULL){
            printf("Text file does not exist");
            exit(1);
        }
    } else {
        error_handling("Invalid Option");
    }

    send(sock, filename, strlen(filename)+1, 0);
    str_len = recv(sock, filename, BUFSIZE, 0);
    filename[str_len] = 0;
    strcat(filename, ".txt");

    send(sock, AorB, strlen(AorB) + 1, 0);

    if (strcmp(AorB, "ascii") == 0) {
        rf = fopen(filename, "w");
    } else if (strcmp(AorB, "binary") == 0) {
        rf = fopen(filename, "wb");
    } else {
        error_handling("Invalid Option");
    }

    while (fgets(message, BUFSIZE, f)){
        send(sock, message, strlen(message) + 1, 0);

        str_len = recv(sock, message, BUFSIZE, 0);
        message[str_len] = 0;
        fputs(message, rf);
    }

	return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}