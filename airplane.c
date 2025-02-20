#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>



/////////////////////////////////////////////
// Struct para armazenar os dados dos aviões
typedef struct Data{
    char token[51];
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

Data *createData(char *empresa, int lugaresMax, char *token);


void pushToListAirplane(Airplane *airplane, Data *data);

void deleteInList(Airplane *airplane, char *token);

char *createToken(Airplane *airplane);
void printList(Airplane *airplane);

// funções para manipular arquivos

void fromFileToList(Airplane *airplane, char *path);
void fromListToFileAirplane(Airplane *airplane, char *path);

////////////////////////////////////////////////////

////////////////////////////////////////////////////

// Funções e estruturas para gerar e manipular rotas aereas

const char Estados[][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", "PB", "PR", "PE", "PI", "RJ", "RN", "RS", "RO", "RR", "SC", "SP", "SE", "TO"};



typedef struct DataRoutes{
    char routeToken[20];
    char aviaoToken[51];
    char AviaoEmpresa[50];
    int lugaresMax;
    int lugaresDisponiveis;
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
    int size;
}Routes;




void pushFromListRoutesToFile(Routes *routes, char *path);

void generateRoute(No *airplane, Routes *routes, time_t leave, int origemIndex, int destinoIndex);
char *createRouteToken(Routes *routes);
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
                printf("Digite o token do aviao a ser removido: ");
                char token[50];
                fgets(token, 50, stdin);
                while(getchar() != '\n');

                printf("\nTem certeza que deseja remover o aviao:%s (s/n)?", token);
                
                char confirm;
                scanf("%c", &confirm);

                while(getchar() != '\n');

                if(confirm != 's' && confirm != 'S'){
                    printf("Remocao cancelada\n");
                    
                    break;
                }
                deleteInList(airplane, token);
                

                break;
            case '3':
                char empresa[50];
                
                char *Token = createToken(airplane);

                printf("Digite o nome da empresa: ");
                fgets(empresa, 50, stdin);
                while(getchar() != '\n');

                Data *data = createData(empresa, 50, Token);
                pushToListAirplane(airplane, data);
                free(Token);
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
                
                time_t leave = time(NULL);
                
                while(current){
                    int origemIndex = 0, destinoIndex = 0;

                    for(int i = 0; i < 500; i++){

                        //passa o index de origem do aviao. dessa forma, garante que o avião passe por todas as regioes
                        
                        
                        while(true){
                            destinoIndex = rand() % 26;
                            if(origemIndex != destinoIndex){
                                break;
                            }
                        
                        }


                        

                        generateRoute(current, routes, leave, origemIndex, destinoIndex);

                        origemIndex = destinoIndex;
                        
                        leave += 12*60*60;
                    }
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

Data *createData(char *empresa, int lugaresMax, char *token){
    if(!empresa ||  !token ){
        perror("erro em createData");
        return NULL;
    }
    

    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    

    strcpy(data->token, token);
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

void deleteInList(Airplane *airplane, char *token) {
    if (!airplane || !token) {
        perror("erro em deleteInList");
        exit(1);
    }

    No *current = airplane->ini;
    No *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->data->token, token) == 0) {
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

    printf("Avião com o token especificado não encontrado\n");
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
        printf("Token:%s\nlugaresMax:%i\nEmpresa:%s\n", current->data->token, current->data->lugaresMax, current->data->empresa);
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
    char token[52];
    
    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário
    */
    while(fgets(token, 52, file) != NULL){
        fgets(lugaresMax, 4, file);
        fgets(empresa, 51, file);
        
        strtok(empresa, "\n");
        strtok(lugaresMax, "\n");
        strtok(token, "\n");
        int  maxLugar = atoi(lugaresMax);

        Data *data = createData(empresa, maxLugar, token);
        pushToListAirplane(airplane, data);
    }

    fclose(file);
}

//gera um token aleatorio e garante que nao seja repetido
char *createToken(Airplane *airplane){
    if(!airplane){
        perror("erro em createToken");
        exit(1);
    }

    while(true){
        char *token = malloc(sizeof(char) * 51);
        if(!token){
            perror("erro ao alocar memória");
            free(token);
            exit(1);
        }
        
        for(int i = 0; i < 51; i++){
            token[i] = 'A' + rand() % 26;
        }
        token[50] = '\0';

        if(airplane->size == 0){
            return token;
        }

        No *current = airplane->ini;

        while(current){
            if(strcmp(current->data->token, token) == 0){
                break;
            }
            current = current->next;
        }

        // current só será diferente de NULL se não tiver estrapolado o final da lista, logo algum aviao com esse token
        if(current){
            continue;
        }

        return token;
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
        fprintf(file, "%s\n%i\n%s\n", current->data->token, current->data->lugaresMax, current->data->empresa);
        current = current->next;
    }
    fclose(file);
}


//////////////////////////////////////////////////////








/////////////////////////////////////////////////////
// funções para manipular lista de rotas



void generateRoute(No *airplane, Routes *routes, time_t leave, int origemIndex, int destinoIndex){

    if(!airplane || !routes){
        perror("erro em generateRoute");
        return;
    }

    NoRoutes *newRoute = malloc(sizeof(NoRoutes));
    if(!newRoute){
        perror("erro ao alocar memória");
        return;
    }
   


   

    newRoute->dataRoutes = malloc(sizeof(DataRoutes));
    if(!newRoute->dataRoutes){
        perror("erro ao alocar memória");
        return;
    }

    char *routeToken = createRouteToken(routes);
    strcpy(newRoute->dataRoutes->routeToken, routeToken);
    newRoute->dataRoutes->lugaresMax = 50;
    newRoute->dataRoutes->lugaresDisponiveis = 50;
    strcpy(newRoute->dataRoutes->aviaoToken, airplane->data->token);
    strcpy(newRoute->dataRoutes->AviaoEmpresa, airplane->data->empresa);
    strcpy(newRoute->dataRoutes->origem, Estados[origemIndex]);
    strcpy(newRoute->dataRoutes->destino, Estados[destinoIndex]);


    
  newRoute->dataRoutes->dateLeave = leave; 
  

  time_t chegada = leave + (3 + (rand() % 7)) * 60 * 60;
  newRoute->dataRoutes->dateArrive = chegada;
 
    newRoute->next = NULL;

    if(routes->ini == NULL){
        routes->ini = newRoute;
        routes->end = newRoute;
    }else{
        routes->end->next = newRoute;
        routes->end = newRoute;
    }

    routes->size++;
    free(routeToken);
}

char *createRouteToken(Routes *routes){
    if(!routes){
        perror("erro em createToken");
        exit(1);
    }

    while(true){
        char *token = malloc(sizeof(char) * 20);
        if(!token){
            perror("erro ao alocar memória");
            free(token);
            exit(1);
        }
        
        for(int i = 0; i < 19; i++){
            token[i] = 'A' + rand() % 26;
        }
        token[19] = '\0';

        if(routes->size == 0){
            return token;
        }

        NoRoutes *current = routes->ini;

        while(current){
            if(strcmp(current->dataRoutes->routeToken, token) == 0){
                break;
            }
            current = current->next;
        }

        if(current){
            continue;
        }

        return token;
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

        char saida_str[80];
        char chegada_str[80];
  
        strftime(saida_str, sizeof(saida_str), "%c", localtime(&current->dataRoutes->dateLeave));
        strftime(chegada_str, sizeof(chegada_str), "%c", localtime(&current->dataRoutes->dateArrive));
  
        fprintf(file, "%s\n%s\n%s\n%i\n%i\n%s\n%s\n%s\n%s\n", 
                current->dataRoutes->routeToken,
                current->dataRoutes->aviaoToken, 
                current->dataRoutes->AviaoEmpresa, 
                current->dataRoutes->lugaresMax,
                current->dataRoutes->lugaresDisponiveis,
                current->dataRoutes->origem, 
                current->dataRoutes->destino, 
                saida_str, 
                chegada_str);
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