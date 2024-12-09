#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


// Struct para armazenar os dados dos aviões
typedef struct Data{
    char tokey[51];
    char empresa[50];
    int lugaresMax;
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

Data *createData(char *empresa, int lugaresMax, char *tokey){
    if(!empresa ||  !tokey ){
        perror("erro em createData");
        return NULL;
    }
    

    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    

    strcpy(data->tokey, tokey);
    strcpy(data->empresa, empresa);
    data->lugaresMax = lugaresMax;
    
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
    
    while(current != NULL){
        
        if(strcmp(current->data->tokey, tokey) == 0){
            No *temp = current;
            current = current->next;
            airplane->size--;
            
            if(airplane->ini == temp){
                airplane->ini = current;
            }
            printf("\nACHOU!\n");
            exit(1);
            free(temp->data);
            free(temp);
            
        }
        current = current->next;
    }


   
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
        printf("\n////////////////////////////////////////////////////////\n");
        printf("Tokey:%s\nlugaresMax:%i\nEmpresa:%s\n", current->data->tokey, current->data->lugaresMax, current->data->empresa);
        printf("////////////////////////////////////////////////////////\n");
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

    char empresa[51];
    char lugaresMax[4];
    char tokey[52];
    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário
    */
    while(fgets(tokey, 52, file) != NULL){
        fgets(lugaresMax, 4, file);
        fgets(empresa, 51, file);
        
        strtok(empresa, "\n");
        strtok(lugaresMax, "\n");
        strtok(tokey, "\n");
        int  maxLugar = atoi(lugaresMax);
        
        Data *data = createData(empresa, maxLugar, tokey);
        pushToList(airplane, data);
    }

    fclose(file);
}

//gera um tokey aleatorio e garante que nao seja repetido
char *createTokey(Airplane *airplane){
    if(!airplane){
        perror("erro em createTokey");
        exit(1);
    }

    while(true){
        char *tokey = malloc(sizeof(char) * 51);
        if(!tokey){
            perror("erro ao alocar memória");
            free(tokey);
            exit(1);
        }
        
        for(int i = 0; i < 51; i++){
            tokey[i] = 'A' + rand() % 26;
        }
        tokey[50] = '\0';

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
        fprintf(file, "%s\n%i\n%s\n", current->data->tokey, current->data->lugaresMax, current->data->empresa);
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
        printf("4 - Sair sem salvar\n");
        printf("5 - Salvar e sair\n");
        
        char op;
        
        scanf("%c", &op);
        while(getchar() != '\n');

        switch(op){
            
            case '1':
                printList(airplane);
                break;
            case '2':
                printf("Digite o tokey do aviao a ser removido: ");
                char tokey[51];
                fgets(tokey, 51, stdin);
                while(getchar() != '\n');

                printf("\nTem certeza que deseja remover o aviao %s? ", tokey);
                
                char confirm;
                scanf("%c", &confirm);

                while(getchar() != '\n');

                if(confirm != 's' && confirm != 'S'){
                    printf("Remocao cancelada\n");
                    
                    break;
                }
                deleteInList(airplane, tokey);
                printf("Aviao removido com sucesso\n");

                break;
            case '3':
                char empresa[50];
                
                char *Tokey = createTokey(airplane);

                printf("Digite o nome da empresa: ");
                fgets(empresa, 50, stdin);
                while(getchar() != '\n');

                Data *data = createData(empresa, 50, Tokey);
                pushToList(airplane, data);
                free(Tokey);
                printf("Aviao adicionado com sucesso\n");
                break;
            case '4':
                printf("Saindo sem salvar\n");
                free(airplane);
                exit(0);
            case '5':
                printf("Salvando e saindo\n");
                fromListToFile(airplane, path);
                free(airplane);
                exit(0);
            default:
                printf("Opcao invalida\n");
                break;
        }
        
    }
    
}