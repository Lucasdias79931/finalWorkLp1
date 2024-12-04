#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


// Struct para armazenar os dados dos aviões
typedef struct Data{
    char *tokey;
    char *empresa;
    char *lugaresMax;
    char *disponiveis;
}Data;

typedef struct No{
    Data *data;
    struct No *next;
}No;

typedef struct Airplane{
    No *ini;
    No *end;
    int size;
}Airplane;




// funções para manipular lista de avioes

Data *createData(char *empresa, char *lugaresMax, char *disponiveis, char *tokey){
    if(!empresa || !lugaresMax || !tokey || !disponiveis){
        perror("erro em createData");
        return NULL;
    }
    

    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    data->tokey = malloc(strlen(tokey) + 1);
    if(!data->tokey){
        perror("erro ao alocar memória");
        free(data->tokey);
        free(data);
        return NULL;
    }

    data->empresa = malloc(strlen(empresa) + 1);
    if(!data->empresa){
        perror("erro ao alocar memória");
        free(data->empresa);
        free(data->tokey);
        free(data);
        return NULL;
    }

    data->lugaresMax = malloc(strlen(lugaresMax) + 1);
    if(!data->lugaresMax){
        perror("erro ao alocar memória");
        free(data->lugaresMax);
        free(data->empresa);
        free(data->tokey);
        free(data);
        return NULL;
    }

    data->disponiveis = malloc(strlen(disponiveis) + 1);
    if(!data->disponiveis){
        perror("erro ao alocar memória");
        free(data->disponiveis);
        free(data->lugaresMax);
        free(data->empresa);
        free(data->tokey);
        free(data);
        return NULL;
    }

    strcpy(data->tokey, tokey);
    strcpy(data->empresa, empresa);
    strcpy(data->lugaresMax, lugaresMax);
    strcpy(data->disponiveis, disponiveis);
    
    return data;

}


void pushToList(Airplane *airplane, Data *data){
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

   airplane->size++;
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
            free(aux->data->lugaresMax);
            free(aux->data->empresa);
            free(aux->data->disponiveis);
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
        perror("erro em printList");
        return;
    }

    if(airplane->size == 0){
        printf("Nenhum aviao cadastrado\n");
        return;
    }

    No *current = airplane->ini;
    while(current){
        printf("Tokey: %s\nlugaresMax: %s\nLugares disponiveis: %s\nEmpresa: %s\n", current->data->tokey, current->data->lugaresMax, current->data->disponiveis, current->data->empresa);
        current = current->next;
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
    char lugaresMax[3];
    char lugaresDisponiveis[3];
    char tokey[50];

    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário
    */
    while(fgets(tokey, 50, file)){
        fgets(lugaresMax, 3, file);
        fgets(lugaresDisponiveis, 3, file);
        fgets(empresa, 50, file);
        Data *data = createData(empresa, lugaresMax, lugaresDisponiveis, tokey);
        pushToList(airplane, data);
    }

    fclose(file);
}

//gera um tokey aleatorio e garante que nao seja repetido
char *creatTokey(Airplane *airplane){
    if(!airplane){
        perror("erro em creatTokey");
        exit(1);
    }

    while(true){
        char *tokey = malloc(sizeof(char) * 50);
        int i;
        for(i = 0; i < 50; i++){
            tokey[i] = 'A' + rand() % 26;
        }
        tokey[i] = '\0';

        if(airplane->size == 0){
            return tokey;
        }

        No *current = airplane->ini;

        while(current){
            if(strcmp(current->data->tokey, tokey) == 0){
                break;
            }
            current = current->next;
        }

        // current só será diferente de NULL se não tiver estrapolado o final da lista, logo algum aviao com esse tokey
        if(current){
            continue;
        }

        return tokey;
    }

   
    
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
        fprintf(file, "%s\n%s\n%s\n%s", current->data->tokey, current->data->lugaresMax,current->data->disponiveis, current->data->empresa);
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

    airplane->ini = NULL;
    airplane->end = NULL;
    airplane->size = 0;


    char path[] = "DB/airplaneM.txt";

    fromFileToList(airplane, path);

    while(true){
        printf("Escolha uma opcao:\n");
        printf("1 - Listar avioes\n");
        printf("2 - Remover aviao\n");
        printf("3 - Adicionar aviao\n");
        printf("4 - Sair\n");
        
        char op;
        
        scanf("%c", &op);
        while(getchar() != '\n');

        switch(op){
            
            case '1':
                printList(airplane);
                break;
            case '2':
                
                break;
            case '3':
                char empresa[50];
                char lugaresMax[] = "50";
                char lugaresDisponiveis[] = "50";
                char *tokey = creatTokey(airplane);

                printf("Digite o nome da empresa: ");
                fgets(empresa, 50, stdin);
                while(getchar() != '\n');

                Data *data = createData(empresa, lugaresMax, lugaresDisponiveis, tokey);
                pushToList(airplane, data);
                free(tokey);
                break;
            case '4':
                fromListToFile(airplane, path);
                free(airplane);
                exit(0);
            default:
                printf("Opcao invalida\n");
                break;
        }
        
    }
    
}