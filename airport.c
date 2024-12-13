#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


// Estruturas e funçãos para manipular dados do cliente


typedef struct Data{
    char nome[100];
    char cpf[15];
    char email[100];
    char telefone[15];
    char idade[15];
    char password[50];
}Data;

typedef struct noCliente{
    Data *data;
    struct noCliente *next;
}NoCliente;

typedef struct Cliente{
    NoCliente *ini;
    NoCliente *end;
}ClienteList;

Data *createData(char *nome, char *cpf, char *email, char *telefone, char *idade, char *password){
    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    strcpy(data->nome, nome);
    strcpy(data->cpf, cpf);
    strcpy(data->email, email);
    strcpy(data->telefone, telefone);
    strcpy(data->idade, idade);
    strcpy(data->password, password);

    return data;
}
void pushToListClient(ClienteList *cliente, Data *data){

    if(!cliente || !data){
        perror("erro em pushToListClient");
        exit(1);
    }

    NoCliente *new = malloc(sizeof(NoCliente));
    if(!new){
        perror("erro ao alocar memória");
        exit(1);
    }

    new->data = data;
    new->next = NULL;

    if(!cliente->ini){
        cliente->ini = new;
        cliente->end = new;
    }else{
        cliente->end->next = new;
        cliente->end = new;
    }
}

void fromFileToClientList(ClienteList *cliente, char *path){

    if(!cliente){
        perror("erro em fromFileToClientList");
        exit(1);
    }

    FILE *file = fopen(path, "r");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    char nome[100];
    char cpf[15];
    char email[100];
    char telefone[15];
    char idade[15];
    char password[50];

    while(fgets(nome, 100, file) != NULL){
        fgets(cpf, 15, file);
        fgets(email, 100, file);
        fgets(telefone, 15, file);
        fgets(idade, 15, file);
        fgets(password, 50, file);
        
        strtok(nome, "\n");
        strtok(cpf, "\n");
        strtok(email, "\n");
        strtok(telefone, "\n");
        strtok(idade, "\n");
        strtok(password, "\n");
        Data *data = createData(nome, cpf, email, telefone, idade, password);
        pushToListClient(cliente, data);
    }
    fclose(file);

}

// retorna true se o cpf for encontrado
bool verifyCpf(ClienteList *cliente, char *cpf){
    NoCliente *current = cliente->ini;
    while(current != NULL){
        if(strcmp(current->data->cpf, cpf) == 0){
            return true;
        }
        current = current->next;
    }
    return false;
}

// retorna true se o cpf e a senha forem encontrados
bool verifyPassword(ClienteList *cliente, char *cpf, char *password){
    NoCliente *current = cliente->ini;
    while(current != NULL){
        
        if(strcmp(current->data->cpf, cpf) == 0 && strcmp(current->data->password, password) == 0){
            printf("Logado com sucesso\n");
            exit(1);
            return true;
        }
        current = current->next;
    }
    return false;
}


void deleteClient(ClienteList *cliente, char *cpf){

    NoCliente *current = cliente->ini;
    NoCliente *prev = NULL;

    while(current != NULL){
        if(strcmp(current->data->cpf, cpf) == 0){
            if(prev == NULL){
                cliente->ini = current->next;
            }else{
                prev->next = current->next;
            }
            free(current->data);
            free(current);
            printf("Cliente removido com sucesso\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Cliente nao encontrado\n");
}

void fromListToFileClient(ClienteList *cliente, char *path){

    if(!cliente){
        perror("erro em fromListToFileClient");
        exit(1);
    }

    FILE *file = fopen(path, "w");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }
   

    NoCliente *current = cliente->ini;
   
    
    while(current != NULL){
        fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s\n", current->data->nome, current->data->cpf, current->data->email, current->data->telefone, current->data->idade, current->data->password);
        current = current->next;
    }
    fclose(file);
}
void freeClientList(ClienteList *cliente){
    NoCliente *current = cliente->ini;
    while(current != NULL){
        NoCliente *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

// Estruturas e funçãos para manipular dados das rotas

typedef struct DataRoutes{
    char aviaoTokey[51];
    char AviaoEmpresa[50];
    int lugaresMax;
    int lugaresDisponiveis;
    char origem[3];
    char destino[3];
    char dateLeave[50];
    char dateArrive[50];
}DataRoutes;

typedef struct NoRoutes{
    DataRoutes *dataRoutes;
    struct NoRoutes *next;
}NoRoutes;

typedef struct Routes{
    NoRoutes *ini;
    NoRoutes *end;
}Routes;

DataRoutes *createDataRoutes(char *aviaoTokey, char *AviaoEmpresa, int maxLugar, int lugaresDisp, char *origem, char *destino, char *dateLeave, char *dateArrive){
    DataRoutes *data = malloc(sizeof(DataRoutes));
    if(!data){
        perror("erro ao alocar memória");
        return NULL;
    }
    strcpy(data->aviaoTokey, aviaoTokey);
    strcpy(data->AviaoEmpresa, AviaoEmpresa);
    data->lugaresMax = maxLugar;
    data->lugaresDisponiveis = lugaresDisp;
    strcpy(data->origem, origem);
    strcpy(data->destino, destino);
    strcpy(data->dateLeave, dateLeave);
    strcpy(data->dateArrive, dateArrive);
    return data;
}
void pushToListRoutes(Routes *routes, DataRoutes *data){
    if (routes == NULL)
    {
        perror("erro em pushToListRoutes");
        exit(1);
    }
    
    
    NoRoutes *newRoute = malloc(sizeof(NoRoutes));
    if(!newRoute){
        perror("erro ao alocar memória");
        return;
    }
    newRoute->dataRoutes = data;
    newRoute->next = NULL;
    if(routes->ini == NULL){
        routes->ini = newRoute;
        routes->end = newRoute;
    }else{
        routes->end->next = newRoute;
        routes->end = newRoute;
    }
}
void fromFileToListRoutes(Routes *routes, char *path){
    FILE *file = fopen(path, "r");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    char aviaoTokey[51];
    int lugaresMax;
    int lugaresDisponiveis;
    char AviaoEmpresa[50];
    char origem[4];
    char destino[4];
    char dateLeave[50];
    char dateArrive[50];
    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário

    */
    while(fgets(aviaoTokey, 51, file) != NULL){
        char lugaresM[4];
        char lugaresD[4];
        fgets(AviaoEmpresa, 50, file);
        fgets(lugaresM, 4, file);
        fgets(lugaresD, 4, file);
        fgets(origem, 4, file);
        fgets(destino, 4, file);
        fgets(dateLeave, 50, file);
        fgets(dateArrive, 50, file);

        strtok(aviaoTokey, "\n");
        strtok(lugaresM, "\n");
        strtok(lugaresD, "\n");
        strtok(AviaoEmpresa, "\n");
        strtok(origem, "\n");
        strtok(destino, "\n");
        strtok(dateLeave, "\n");
        strtok(dateArrive, "\n");

        
        int  maxLugar = atoi(lugaresM);
        int  lugaresDisp = atoi(lugaresD);
        DataRoutes *data = createDataRoutes(aviaoTokey, AviaoEmpresa, maxLugar, lugaresDisp, origem, destino, dateLeave, dateArrive);
        pushToListRoutes(routes, data);


    }
}

void fromListToFileRoutes(Routes *routes, char *path){

    if(!routes){
        perror("erro em fromListToFileRoutes");
        exit(1);
    }

    FILE *file = fopen(path, "w");
    if(!file){
        perror("Erro ao abrir o arquivo");
        return;
    }

    NoRoutes *current = routes->ini;
    while(current != NULL){
        fprintf(file, "%s\n%s,\n%i\n%i\n%s\n%s\n%s%s", 
                current->dataRoutes->aviaoTokey, 
                current->dataRoutes->AviaoEmpresa, 
                current->dataRoutes->lugaresMax,
                current->dataRoutes->lugaresDisponiveis,
                current->dataRoutes->origem, 
                current->dataRoutes->destino, 
                current->dataRoutes->dateLeave, 
                current->dataRoutes->dateArrive);
        current = current->next;
    }
    fclose(file);
}


void printByRegion(int region){
    switch (region){
    case 1:
        printf("Região Nordeste\n");
        printf("al (Alagoas)\tba (Bahia)\tce (Ceará)\tpb (Paraíba)\tpi (Piauí)\trn (Rio Grande do Norte)\tpe (Pernambuco)\n");
        printf("ma (Maranhão)\tse (Sergipe)");
        break;
    
    case 2:
        printf("Região Norte\n");
        printf("ac (Acre)\tam (Amazonas)\tap (Amapá)\tma (Maranhão não, apenas para o norte, é o Amapá, Amazonas, Acre, Roraima, Rondônia, Tocantins e Pará)\tpa (Pará)\trr (Roraima)\tror (Rondônia)\tto (Tocantins)");
        break;
    
    case 3:
        printf("Região Sul\n");
        printf("pr (Paraná)\trs (Rio Grande do Sul)\tsc (Santa Catarina)");
        break;
    
    case 4:
        printf("Região Sudeste\n");
        printf("es (Espírito Santo)\tmg (Minas Gerais)\trj (Rio de Janeiro)\tspe (São Paulo)");
        break;
    
    case 5:
        printf("Região Centro-Oeste\n");
        printf("df (Distrito Federal)\tgo (Goiás)\tms (Mato Grosso)\tmt (Mato Grosso do Sul)");
        break;
    
    default:
        printf("Região inválida");
        break;
    }
}

void printAllRoutes(Routes *routes){
    if(!routes){
        perror("erro em printAllRoutes");
        exit(1);
    }
    NoRoutes *current = routes->ini;
    while(current != NULL){
        printf("%s\n%s,\n%i\n%i\n%s\n%s\n%s\n%s\n", 
                current->dataRoutes->aviaoTokey, 
                current->dataRoutes->AviaoEmpresa, 
                current->dataRoutes->lugaresMax,
                current->dataRoutes->lugaresDisponiveis,
                current->dataRoutes->origem, 
                current->dataRoutes->destino, 
                current->dataRoutes->dateLeave, 
                current->dataRoutes->dateArrive);
        current = current->next;
    }
}
int main(){
    ClienteList *clients = malloc(sizeof(ClienteList));
    if(!clients){
        perror("erro ao alocar memória");
        free(clients);
        exit(1);
    }

    clients->ini = NULL;
    clients->end = NULL;


    char path[] = "DB/clientes.txt";

    fromFileToClientList(clients, path);
    /*
    while(true){
        printf("Escolha uma opcao:\n");
        printf("1 - logar\n");
        printf("2 - cadastrar\n");
        printf("3 - deletar\n");
        printf("4 - sair sem salvar\n");
        printf("5 - salvar e sair\n");
        bool next = false;
        char op;
        scanf("%c", &op);
        while (getchar() != '\n');
        
        switch(op){
            case '1':
                char Password[50];
                char Cpf[15];

                printf("Digite o cpf: ");
                fgets(Cpf, 15, stdin);
                while(getchar() != '\n');
                printf("Digite a senha: ");
                fgets(Password, 50, stdin);
                while(getchar() != '\n');

                strtok(Cpf, "\n");
                strtok(Password, "\n");
                if(verifyPassword(clients, Cpf, Password) == true){
                    printf("Logado com sucesso\n");
                    next = true;
                }else{
                    printf("Cpf ou senha incorretos\n");
                }
                break;
            case '2':
                char nome[100];
                char cpf[15];
                char email[100];
                char telefone[15];
                char idade[15];
                char password[50];

                printf("Digite o nome: ");
                fgets(nome, 100, stdin);
                while(getchar() != '\n' );
                printf("Digite o cpf: ");
                fgets(cpf, 15, stdin);
                if(verifyCpf(clients, cpf) == true){
                    printf("Cpf ja cadastrado\n");
                    break;
                }

                while(getchar() != '\n');
                printf("Digite o email: ");
                fgets(email, 100, stdin);
                while(getchar() != '\n');
                printf("Digite o telefone: ");
                fgets(telefone, 15, stdin);
                while(getchar() != '\n');
                printf("Digite a idade: ");
                fgets(idade, 15, stdin);
                while(getchar() != '\n');
                printf("Digite a senha: ");
                fgets(password, 50, stdin);
                while(getchar() != '\n');

                
                strtok(nome, "\n");
                strtok(cpf, "\n");
                strtok(email, "\n");
                strtok(telefone, "\n");
                strtok(idade, "\n");
                strtok(password, "\n");

                Data *data = createData(nome, cpf, email, telefone, idade, password);
                pushToListClient(clients, data);


                break;
            case '3':
                char delCpf[15];
                printf("Digite o cpf: ");
                fgets(delCpf, 15, stdin);
                while(getchar() != '\n');
                strtok(delCpf, "\n");
                
                deleteClient(clients, delCpf);
                break;
            case '4':
                exit(0);
                break;
            case '5':
                fromListToFileClient(clients, path);
                freeClientList(clients);
                exit(0);
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }

        if(next)break;
    }
    */
    char pathRoutes[] = "DB/Routes.txt";

    Routes *routes = malloc(sizeof(Routes));
    if(!routes){
        perror("erro ao alocar memória");
        free(routes);
        exit(1);
    }

    routes->ini = NULL;
    routes->end = NULL;
    
    fromFileToListRoutes(routes, pathRoutes);

    printAllRoutes(routes);
    
    
    while (true){
        char leave[3];
        char arrive[3];
        bool idaVolta = false;
        char op;

        printf("Escolha uma opcao:\n");
        printf("1 - ida e volta\n");
        printf("2 - somente ida\n");
        printf("4 - Listar passagens compradas\n");
        printf("5 - sair\n");
        scanf("%c", &op);
        while (getchar() != '\n');

        switch(op){
            case '1':
                idaVolta = true;
                break;
            case '2':
                idaVolta = false;
                break;
            case '4':
                
                break;
            case '5':
                exit(0);
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
        
        
        
    }
    
 
    return 0;
}
