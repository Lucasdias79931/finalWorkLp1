#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


// Struct para armazenar os dados dos aviões
typedef struct{
    char *tokey;
    char *empresa;
    char *lugares;
}Data;

typedef struct{
    Data *data;
    struct No *next;
}No;

typedef struct{
    No *ini;
    No *end;
}Airplane;




// funções para manipular lista de avioes

Data *createData(char *empresa, char *lugares, char *tokey){
    if(!empresa || !lugares || !tokey){
        perror("erro em createData");
        return NULL;
    }
    

    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    data->empresa = malloc(strlen(empresa) + 1);
    if(!data->empresa){
        perror("erro ao alocar memória");
        free(data->empresa);
        free(data);
        return NULL;
    }

    data->lugares = malloc(strlen(lugares) + 1);
    if(!data->lugares){
        perror("erro ao alocar memória");
        free(data->empresa);
        free(data->lugares);
        free(data);
        return NULL;
    }

    strcpy(data->empresa, empresa);
    data->empresa[strlen(empresa)] = '\0';
    strcpy(data->lugares, lugares);
    data->lugares[strlen(lugares)] = '\0';

    data->tokey = malloc(strlen(tokey) + 1);
    if(!data->tokey){
        perror("erro ao alocar memória");
        free(data->empresa);
        free(data->lugares);
        free(data->tokey);
        free(data);
        return NULL;
    }

    strcpy(data->tokey, tokey);
    data->tokey[strlen(tokey)] = '\0';
    return data;

}


void pushToList(Airplane *airplane, No *data){
   if(!airplane || !data){
       perror("erro em pushToList");
       exit(1);
   }

   No *new = malloc(sizeof(No));
   if(!new){
       perror("erro ao alocar memória");
       exit(1);
   }

   new->data = data;
   new->next = NULL;

   if(!airplane->ini){
       airplane->ini = new;
       airplane->end = new;
   }else{
       airplane->end->next = new;
       airplane->end = new;
   }
}

void deleteInList(Airplane *airplane, char *tokey){
    if(!airplane || !tokey){
        perror("erro em deleteInList");
        exit(1);
    }

    No *current = airplane->ini;
    
    while(current->next){
        if(strcmp(current->data->tokey, tokey) == 0){
            No *aux = current;
            current = current->next;

            free(aux->data->tokey);
            free(aux->data->lugares);
            free(aux->data->empresa);
            free(aux->data);
            free(aux);
            printf("Aviao removido com sucesso\n");
            return;
        }
    }

    printf("Aviao nao encontrado\n");
    return;
}

void printList(Airplane *airplane){

    if(!airplane){
        perror("Lista de avioes vazia");
        return;
    }

    No *current = airplane->ini;
    while(current){
        printf("%s %s %s\n", current->data->tokey, current->data->lugares, current->data->empresa);
    }

}

// funções para manipular arquivos

void fromFileToList(Airplane *airplane, char *path){

    FILE *file = fopen(path, "r");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    
}