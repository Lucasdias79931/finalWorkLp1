#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct Data {
    char *tokey;
    char *empresa;
} Data;

char *newTokey(Data data[], int size) {
    while (true) {
        char *tokey = malloc(sizeof(char) * 51);
        for (int i = 0; i < 49; i++) {
            tokey[i] = 'A' + rand() % 26;
        }
        tokey[50] = '\0';

        int i = 0;
        for (i = 0; i < size; i++) {
            if (data[i].tokey != NULL && strcmp(data[i].tokey, tokey) == 0) {
                free(tokey);
                break;
            }
        }

        if (i == size) {
            return tokey;
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
        data[i].tokey = newTokey(data, i);
        data[i].empresa = empresas[rand() % 15];
        fprintf(file, "%s\n%s\n%s\n%s\n", data[i].tokey, "50", "50", data[i].empresa);
    }

    for (int i = 0; i < size; i++) {
        free(data[i].tokey);
    }

    fclose(file);
    return 0;
}
