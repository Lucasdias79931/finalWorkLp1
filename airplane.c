#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Struct para armazenar os dados dos aviões

typedef struct{
    char empresa[50];
    char lugares[3];
}Airplane;



// Manipula os dados dos aviões

// Adiciona um novo avião

void addAirplane(Airplane *airplane, char *destine){
    if(!airplane){
        perror("Dados nulos");
        exit(1);
    }

    FILE *file = fopen("airplaneM.txt", "a");
    
    if(!file){
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    fprintf(file, "\n%s\n%s", airplane->empresa, airplane->lugares);
    fclose(file);
    
}



