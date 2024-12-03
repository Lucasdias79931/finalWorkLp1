#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// Struct para armazenar os dados dos aviões
typedef struct{
    char *empresa;
    char *lugares;
}Data;

typedef struct{
    Data *data;
    struct No *next;
}No;

typedef struct{
    No *head;
    No *tail;
}Airplane;




// funções para manipular lista de avioes

Data *createData(char *empresa, char *lugares){
    if(!empresa || !lugares){
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

   if(!airplane->head){
       airplane->head = new;
       airplane->tail = new;
   }else{
       airplane->tail->next = new;
       airplane->tail = new;
   }
}




