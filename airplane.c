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
    if(!airplane){
        perror("erro em fromFileToList");
        exit(1);
    }

    FILE *file = fopen(path, "r");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    char empresa[50];
    char lugares[3];
    char tokey[50];

    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário
    */
    while(fgets(tokey, 50, file)){
        fgets(lugares, 3, file);
        fgets(empresa, 50, file);
        Data *data = createData(empresa, lugares, tokey);
        pushToList(airplane, data);
    }

    fclose(file);
}


void fromListToFile(Airplane *airplane, char *path){

    if(!airplane){
        perror("erro em fromListToFile");
        exit(1);
    }

    FILE *file = fopen(path, "w");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    No *current = airplane->ini;
    while(current){
        fprintf(file, "%s\n%s\n%s\n", current->data->tokey, current->data->lugares, current->data->empresa);
        current = current->next;
    }
    fclose(file);
}



// função principal

int main(){
    Airplane *airplane = malloc(sizeof(Airplane));
    if(!airplane){
        perror("erro ao alocar memória");
        free(airplane);
        exit(1);
    }


    char path[] = "DB/airplaneM.txt";

    fromFileToList(airplane, path);

    while(true){

    }
    
}