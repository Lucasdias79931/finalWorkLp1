#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct Data {
    char *token;
    char *empresa;
} Data;

char *newToken(Data data[], int size) {
    while (true) {
        char *token = malloc(sizeof(char) * 51);
        for (int i = 0; i < 49; i++) {
            token[i] = 'A' + rand() % 26;
        }
        token[50] = '\0';

        int i = 0;
        for (i = 0; i < size; i++) {
            if (data[i].token != NULL && strcmp(data[i].token, token) == 0) {
                free(token);
                break;
            }
        }

        if (i == size) {
            return token;
        }
    }
}

int main(int argc, char *argv[]) {
    char destine[100];
    if (argc > 1) {
        strcpy(destine, argv[1]);
    } else {
        strcpy(destine, "airplaneM.txt");
    }

    FILE *file = fopen(destine, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    srand(time(NULL));

    char empresas[][50] = {
        "LATAM Airlines",
        "Gol Linhas Aéreas",
        "Azul Linhas Aéreas",
        "American Airlines",
        "Delta Airlines",
        "United Airlines",
        "Air France",
        "Lufthansa",
        "Qatar Airways",
        "Emirates",
        "British Airways",
        "KLM Royal Dutch Airlines",
        "Singapore Airlines",
        "Turkish Airlines",
        "Copa Airlines"
    };

    Data data[200] = {0};
    int size = 200;

    for (int i = 0; i < size; i++) {
        data[i].token = newToken(data, i);
        data[i].empresa = empresas[rand() % 15];
        fprintf(file, "%s\n%s\n%s\n", data[i].token, "50", data[i].empresa);
    }

    for (int i = 0; i < size; i++) {
        free(data[i].token);
    }

    fclose(file);
    return 0;
}
