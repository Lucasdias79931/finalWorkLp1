#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>



/////////////////////////////////////////////
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

Data *createData(char *empresa, int lugaresMax, char *tokey);


void pushToListAirplane(Airplane *airplane, Data *data);

void deleteInList(Airplane *airplane, char *tokey);

char *createTokey(Airplane *airplane);
void printList(Airplane *airplane);

// funções para manipular arquivos

void fromFileToList(Airplane *airplane, char *path);
void fromListToFileAirplane(Airplane *airplane, char *path);

////////////////////////////////////////////////////

////////////////////////////////////////////////////

// Funções e estruturas para gerar e manipular rotas aereas

const char Estados[][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", "PB", "PR", "PE", "PI", "RJ", "RN", "RS", "RO", "RR", "SC", "SP", "SE", "TO"};



typedef struct DataRoutes{
    char aviaoTokey[51];
    char AviaoEmpresa[50];
    int lugaresMax;
    char origem[3];
    char destino[3];
    time_t dateLeave;
    time_t dateArrive;
}DataRoutes;

typedef struct NoRoutes{
    DataRoutes *dataRoutes;
    struct NoRoutes *next;
}NoRoutes;

typedef struct Routes{
    NoRoutes *ini;
    NoRoutes *end;
}Routes;




void pushFromListRoutesToFile(Routes *routes, char *path);

void generateRoute(No *airplane, Routes *routes);

void freeListRoutes(Routes **routes);

// função principal

int main(){
    Airplane *airplane = malloc(sizeof(Airplane));
    srand(time(NULL));
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
        printf("4 - Gerar rotas\n");
        printf("5 - sair\n");

        
        char op;
        
        scanf("%c", &op);
        while(getchar() != '\n');

        switch(op){
            
            case '1':
                printList(airplane);
                break;
            case '2':
                printf("Digite o tokey do aviao a ser removido: ");
                char tokey[50];
                fgets(tokey, 50, stdin);
                while(getchar() != '\n');

                printf("\nTem certeza que deseja remover o aviao:%s (s/n)?", tokey);
                
                char confirm;
                scanf("%c", &confirm);

                while(getchar() != '\n');

                if(confirm != 's' && confirm != 'S'){
                    printf("Remocao cancelada\n");
                    
                    break;
                }
                deleteInList(airplane, tokey);
                

                break;
            case '3':
                char empresa[50];
                
                char *Tokey = createTokey(airplane);

                printf("Digite o nome da empresa: ");
                fgets(empresa, 50, stdin);
                while(getchar() != '\n');

                Data *data = createData(empresa, 50, Tokey);
                pushToListAirplane(airplane, data);
                free(Tokey);
                printf("Aviao adicionado com sucesso\n");
                break;
            case '4':
                Routes *routes = malloc(sizeof(Routes));
                if(!routes){
                    perror("erro ao alocar memória");
                    free(routes);
                    exit(1);
                }

                routes->ini = NULL;
                routes->end = NULL;

                char pathRoutes[] = "DB/Routes.txt";

                No *current = airplane->ini;
                printf("\nGerando rotas\n");
                while(current){
                    generateRoute(current, routes);
                    current = current->next;

                }

                printf("\nRotas geradas com sucesso\n");
                pushFromListRoutesToFile(routes, pathRoutes);
                freeListRoutes(&routes);
                break;
            
            case '5':
                printf("Salvando e saindo\n");
                fromListToFileAirplane(airplane, path);
                free(airplane);
                exit(0);
            default:
                printf("Opcao invalida\n");
                break;
        }
        
    }
    
}


////////////////////////////////////////////////////
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


void pushToListAirplane(Airplane *airplane, Data *data){
   if(!airplane || !data){
       perror("erro em pushToListAirplane");
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

void deleteInList(Airplane *airplane, char *tokey) {
    if (!airplane || !tokey) {
        perror("erro em deleteInList");
        exit(1);
    }

    No *current = airplane->ini;
    No *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->data->tokey, tokey) == 0) {
            if (prev == NULL) {
                airplane->ini = current->next;
            } else {
                prev->next = current->next;
            }

            airplane->size--;

            // Libera a memória do nó removido
            free(current->data);
            free(current);

            printf("Avião removido com sucesso\n");
            return;
        }

       
        prev = current;
        current = current->next;
    }

    printf("Avião com o tokey especificado não encontrado\n");
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
        pushToListAirplane(airplane, data);
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
void fromListToFileAirplane(Airplane *airplane, char *path){

    if(!airplane){
        perror("erro em fromListToFileAirplane");
        exit(1);
    }

    FILE *file = fopen(path, "w");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    No *current = airplane->ini;
    while(current != NULL){
        fprintf(file, "%s\n%i\n%s\n", current->data->tokey, current->data->lugaresMax, current->data->empresa);
        current = current->next;
    }
    fclose(file);
}


//////////////////////////////////////////////////////








/////////////////////////////////////////////////////
// funções para manipular lista de rotas



void generateRoute(No *airplane, Routes *routes){

    if(!airplane || !routes){
        perror("erro em generateRoute");
        return;
    }

    NoRoutes *newRoute = malloc(sizeof(NoRoutes));
    if(!newRoute){
        perror("erro ao alocar memória");
        return;
    }
   


    int origemIndex  = 0;
    int destinoIndex = 0;
    while (true){
        origemIndex  = rand() % 26;
        destinoIndex = rand() % 26;
        if(origemIndex != destinoIndex){
            break;
        }
    }

    newRoute->dataRoutes = malloc(sizeof(DataRoutes));
    if(!newRoute->dataRoutes){
        perror("erro ao alocar memória");
        return;
    }

    strcpy(newRoute->dataRoutes->aviaoTokey, airplane->data->tokey);
    strcpy(newRoute->dataRoutes->AviaoEmpresa, airplane->data->empresa);
    strcpy(newRoute->dataRoutes->origem, Estados[origemIndex]);
    strcpy(newRoute->dataRoutes->destino, Estados[destinoIndex]);


    
    
    time_t agora = time(NULL);
    newRoute->dataRoutes->dateLeave = agora + (rand() % 31536000); // Até 1 ano no futuro

    // Garantir que dateArrive é entre 3 e 9 horas após dateLeave
    newRoute->dataRoutes->dateArrive = newRoute->dataRoutes->dateLeave + 10800 + (rand() % 21601);


   
   
    newRoute->next = NULL;

    if(routes->ini == NULL){
        routes->ini = newRoute;
        routes->end = newRoute;
    }else{
        routes->end->next = newRoute;
        routes->end = newRoute;
    }
}


void pushFromListRoutesToFile(Routes *routes, char *path){
    if(!routes){
        perror("erro em pushFromListRoutesToFile");
        exit(1);
    }

    FILE *file = fopen(path, "w");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    NoRoutes *current = routes->ini;
    
    while(current != NULL){

        
        fprintf(file, "%s\n%s\n%s\n%s\n%s%s", 
                current->dataRoutes->aviaoTokey, 
                current->dataRoutes->AviaoEmpresa, 
                current->dataRoutes->origem, 
                current->dataRoutes->destino, 
                ctime(&current->dataRoutes->dateLeave), 
                ctime(&current->dataRoutes->dateArrive));
        current = current->next;
    }
    fclose(file);
}

void freeListRoutes(Routes **routes){
    NoRoutes *current = (*routes)->ini;

    while(current != NULL){
        NoRoutes *next = current->next;
        free(current->dataRoutes);
        free(current);
        current = next;
    }

    free((*routes));
}