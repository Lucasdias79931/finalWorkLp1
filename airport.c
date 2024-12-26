#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Estruturas e funçãos para manipular dados do cliente


typedef struct Data{
    char tokeyCliente[31];
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
    int size;
}ClienteList;


char *createClientTokey(ClienteList *clients){
    if(!clients){
        printf("erro em createTokey");
        perror("ERROR");
        exit(1);
    }

    while(true){
        char *tokey = malloc(sizeof(char) * 30);
        if(!tokey){
            perror("erro ao alocar memória");
            free(tokey);
            exit(1);
        }
        
        for(int i = 0; i < 29; i++){
            tokey[i] = 'A' + rand() % 26;
        }
        tokey[30] = '\0';

        if(clients->size == 0){
            return tokey;
        }

        NoCliente *current = clients->ini;

        while(current){
            if(strcmp(current->data->tokeyCliente, tokey) == 0){
                break;
            }
            current = current->next;
        }

        if(current){
            continue;
        }

        return tokey;
    }

   
    
}

Data *createData(char *tokeyCliente, char *nome, char *cpf, char *email, char *telefone, char *idade, char *password){
    Data *data = malloc(sizeof(Data));
    if(!data){
        perror("erro ao alocar memória");
        free(data);
        return NULL;
    }

    strcpy(data->tokeyCliente, tokeyCliente);
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
    char tokey[31];
    char nome[100];
    char cpf[15];
    char email[100];
    char telefone[15];
    char idade[15];
    char password[50];

    while(fgets(tokey, 31, file) != NULL){
        fgets(nome, 100, file);
        fgets(cpf, 15, file);
        fgets(email, 100, file);
        fgets(telefone, 15, file);
        fgets(idade, 15, file);
        fgets(password, 50, file);
        
        

        tokey[strcspn(tokey, "\n")] = '\0';
        nome[strcspn(nome, "\n")] = '\0';
        cpf[strcspn(cpf, "\n")] = '\0';
        email[strcspn(email, "\n")] = '\0';
        telefone[strcspn(telefone, "\n")] = '\0';
        idade[strcspn(idade, "\n")] = '\0';
        password[strcspn(password, "\n")] = '\0';

        Data *data = createData(tokey, nome, cpf, email, telefone, idade, password);
        
        pushToListClient(cliente, data);
    }
    fclose(file);
    


}

// retorna true do cliente se o cpf for encontrado
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

// retorna o tokey do cliente se o cpf for encontrado
char *verifyPassword(ClienteList *cliente, char *cpf, char *password){
    NoCliente *current = cliente->ini;
    while(current != NULL){
        
        if(strcmp(current->data->cpf, cpf) == 0 && strcmp(current->data->password, password) == 0){
            printf("Logado com sucesso\n");
            
            return current->data->tokeyCliente;
        }
        current = current->next;
    }
    return NULL;
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
        fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",current->data->tokeyCliente, current->data->nome, current->data->cpf, current->data->email, current->data->telefone, current->data->idade, current->data->password);
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

void printAllClients(ClienteList *cliente){
    if(!cliente){
        perror("erro em printAllClient");
        return;
    }
    NoCliente *current = cliente->ini;
    while(current != NULL){
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",current->data->tokeyCliente, current->data->nome, current->data->cpf, current->data->email, current->data->telefone, current->data->idade, current->data->password);
        current = current->next;
    }
}
// Estruturas e funçãos para manipular dados das rotas

typedef struct DataRoutes{
    char routesTokey[20];
    char aviaoTokey[51];
    char AviaoEmpresa[50];
    int lugaresMax;
    int lugaresDisponiveis;
    char origem[3];
    char destino[3];
    char dateLeave[50];
    char dateArrive[50];
    float price;
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

DataRoutes *createDataRoutes(char *routeTokey, char *aviaoTokey, char *AviaoEmpresa, int maxLugar, int lugaresDisp, char *origem, char *destino, char *dateLeave, char *dateArrive){
    DataRoutes *data = malloc(sizeof(DataRoutes));
    if(!data){
        perror("erro ao alocar memória");
        return NULL;
    }

    strcpy(data->routesTokey, routeTokey);
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

    char routeTokey[21];
    char aviaoTokey[51];
    int lugaresMax;
    int lugaresDisponiveis;
    char AviaoEmpresa[51];
    char origem[4];
    char destino[4];
    char dateLeave[50];
    char dateArrive[50];
    srand(time(NULL));
    time_t today = time(NULL);
    time_t nextWeek = today + 7 * 24 * 60 * 60;
    
    
    /* 
        le os dados do arquivo e armazena em uma struct Data
        depois armazena a struct na lista principal que será manipulada pelo usuário

    */
    while(fgets(routeTokey, 21, file) != NULL){
        fgets(aviaoTokey, 51, file);
        char lugaresM[4];
        char lugaresD[4];
        fgets(AviaoEmpresa, 51, file);
        fgets(lugaresM, 4, file);
        fgets(lugaresD, 4, file);
        fgets(origem, 4, file);
        fgets(destino, 4, file);
        fgets(dateLeave, 50, file);
        fgets(dateArrive, 50, file);

        routeTokey[strcspn(routeTokey, "\n")] = '\0';
        aviaoTokey[strcspn(aviaoTokey, "\n")] = '\0';
        AviaoEmpresa[strcspn(AviaoEmpresa, "\n")] = '\0';
        lugaresM[strcspn(lugaresM, "\n")] = '\0';
        lugaresD[strcspn(lugaresD, "\n")] = '\0';
        origem[strcspn(origem, "\n")] = '\0';
        destino[strcspn(destino, "\n")] = '\0';
        dateLeave[strcspn(dateLeave, "\n")] = '\0';
        dateArrive[strcspn(dateArrive, "\n")] = '\0';

        

        
        
        int  maxLugar = atoi(lugaresM);
        int  lugaresDisp = atoi(lugaresD);
        
        
        DataRoutes *data = createDataRoutes(routeTokey, aviaoTokey, AviaoEmpresa, maxLugar, lugaresDisp, origem, destino, dateLeave, dateArrive);

        

        data->price = 300 + (rand() % 2500);
        
        struct tm tm_leave = {0};  
        time_t leave_time;

        if (strptime(data->dateLeave, "%a %b %d %H:%M:%S %Y", &tm_leave) == NULL) {
            printf("Erro ao converter a data\n");
            printf("%s\n", data->dateLeave);
            exit(1);
        }
        leave_time = mktime(&tm_leave);
        

        if(leave_time >= today && leave_time <= nextWeek){
            data->price += data->price * 0.30;
        }
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
        fprintf(file, "%s\n%s\n%s\n%i\n%i\n%s\n%s\n%s\n%s\n",
                current->dataRoutes->routesTokey, 
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





//função apenas para testes
void printAllRoutes(Routes *routes){
    if(!routes){
        perror("erro em printAllRoutes");
        exit(1);
    }
    NoRoutes *current = routes->ini;
    while(current != NULL){
        printf("%s\n%s\n%s\n%i\n%i\n%s\n%s\n%s\n%s\n%f\n",
                current->dataRoutes->routesTokey, 
                current->dataRoutes->aviaoTokey, 
                current->dataRoutes->AviaoEmpresa, 
                current->dataRoutes->lugaresMax,
                current->dataRoutes->lugaresDisponiveis,
                current->dataRoutes->origem, 
                current->dataRoutes->destino, 
                current->dataRoutes->dateLeave, 
                current->dataRoutes->dateArrive,
                current->dataRoutes->price);
        current = current->next;
    }
}
////////////////////////////////////////////////

// Estrutura e funçõeos para manipular as passagens dos clientes

typedef struct dataPassages{
    char routeTokey[20];
    char clientTokey[30];
    float price;
}dataPassages;

typedef struct NoPassages{
    dataPassages *dataPassages;
    struct NoPassages *next;
}NoPassages;

typedef struct Passages{
    NoPassages *ini;
    NoPassages *end;
    int size;
}Passages;

dataPassages *createDataPassages(char *routeTokey, char *clientTokey, float price){
    dataPassages *data = malloc(sizeof(dataPassages));
    if(!data){
        perror("erro ao alocar memória");
        return NULL;
    }
    strcpy(data->routeTokey, routeTokey);
    strcpy(data->clientTokey, clientTokey);
    data->price = price;
    return data;
}
void addPassages(Passages *passages, dataPassages *data){
    NoPassages *newNode = malloc(sizeof(NoPassages));
    if(!newNode){
        perror("erro ao alocar memória");
        return;
    }
    newNode->dataPassages = data;
    newNode->next = NULL;
    if(passages->ini == NULL){
        passages->ini = newNode;
        passages->end = newNode;
    }else{
        passages->end->next = newNode;
        passages->end = newNode;
    }
    passages->size++;
}
void fromFileToListPassages(Passages *passages, char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char routeTokey[21];  
    char clientTokey[31]; 
    char price[10];       

    while (1) {
        // Lê e verifica cada campo individualmente
        if (!fgets(routeTokey, sizeof(routeTokey), file)) break;
        if (!fgets(clientTokey, sizeof(clientTokey), file)) break;
        if (!fgets(price, sizeof(price), file)) break;

        // Remove possíveis '\n' do final das strings
        routeTokey[strcspn(routeTokey, "\n")] = '\0';
        clientTokey[strcspn(clientTokey, "\n")] = '\0';
        price[strcspn(price, "\n")] = '\0';

        // Converte o preço para float
        float priceFloat = atof(price);

        // Cria e adiciona a estrutura de dados
        dataPassages *data = createDataPassages(routeTokey, clientTokey, priceFloat);
        addPassages(passages, data);
    }

    fclose(file);

    
}

void fromListToFilePassages(Passages *passages, char *path){
    FILE *file = fopen(path, "w");
    if(!file){
        perror("erro ao abrir o arquivo");
        exit(1);
    }
    NoPassages *current = passages->ini;
    while(current != NULL){
        fprintf(file, "%s\n%s\n%.4f\n", current->dataPassages->routeTokey, current->dataPassages->clientTokey, current->dataPassages->price);
        current = current->next;
      
    }
    fclose(file);
}

/// Funções para compra 

void printByRegion(){
    int region;
    bool stop = false;
    while (true){
        
        printf("\n1 - Regiao Nordeste\n2 - Regiao Norte\n3 - Regiao Sul\n4 - Regiao Sudeste\n5 - Regiao Centro-Oeste\n");
        scanf("%i", &region);
        while(getchar() != '\n');
        switch (region){
            case 1:
                printf("Região Nordeste\n");
                printf("AL (Alagoas)\tBA (Bahia)\tCE (Ceará)\tPB (Paraíba)\tPI (Piauí)\tRN (Rio Grande do Norte)\tPE (Pernambuco)\n");
                printf("MA (Maranhão)\tSE (Sergipe)");
                stop = true;
                break;
            
            case 2:
                printf("Região Norte\n");
                printf("AC (Acre)\tAM (Amazonas)\tAP (Amapá)\tMA (Maranhão não, apenas para o norte, é o Amapá, Amazonas, Acre, Roraima, Rondônia, Tocantins e Pará)\tPA (Pará)\tRR (Roraima)\tRO (Rondônia)\tTO (Tocantins)");
                stop = true;
                break;
            
            case 3:
                printf("Região Sul\n");
                printf("PR (Paraná)\tRS (Rio Grande do Sul)\tSC (Santa Catarina)");
                stop = true;
                break;
            
            case 4:
                printf("Região Sudeste\n");
                printf("ES (Espírito Santo)\tMG (Minas Gerais)\tRJ (Rio de Janeiro)\tSP (São Paulo)");
                stop = true;
                break;
            
            case 5:
                printf("Região Centro-Oeste\n");
                printf("DF (Distrito Federal)\tGO (Goiás)\tMS (Mato Grosso)\tMT (Mato Grosso do Sul)");
                stop = true;
                break;
            
            default:

                printf("Região inválida");
                getchar();
                break;
            }

            if(stop){
                break;
            }
    }
    
}
void printRoute(DataRoutes *route){
    printf("\n////////////////////////////////////\n");
    printf("Origem:%s\nDestino:%s\nPartida:%s\nChegada:%s\nPreço:%f",
                route->origem, 
                route->destino, 
                route->dateLeave, 
                route->dateArrive,
                route->price);
    printf("\n////////////////////////////////////\n");
}
void fouldRoute(char *leave, char *arrive, const char Estados[][3], bool back){
    
    while (true){
            char sainda[3], chegada[3];
            clearScreen();
            
            if(!back){
                printf("\nEscolha a Regiao de origem\n");
                
                printByRegion();
                printf("\n");
                fgets(leave, 3, stdin);
                while(getchar() != '\n');
               
            }else{
                strcpy(leave, arrive);
            }
            
            
            printf("\nDigite o estado de destino\n");
            
            printByRegion();
            printf("\n");
            fgets(arrive, 3, stdin);
            while(getchar() != '\n');

            int fould = 0;
            //transforma o imput do user em maiusculo
            for(int i = 0; i < 3; i++){
                leave[i] = toupper(leave[i]);
                arrive[i] = toupper(arrive[i]);
            }

            if(strcmp(leave, arrive) == 0){
                printf("Origem e destino iguais\n");
                
                getchar();
                continue;
            }

            for(int i = 0; i < 27; i++){
                if(strcmp(Estados[i], leave) == 0){
                    fould++;
                }
                if(strcmp(Estados[i], arrive) == 0){
                    fould++;
                }
            }
            if(fould == 2){
                printf("Origem: %s\nDestino: %s\n", leave, arrive);
                printf("Presionar enter para continuar\n");
                getchar();
                clearScreen();
                break;
            }else{
                printf("Estado nao encontrado! Preencha novamente\n");
                printf("estados digitados: %s %s\n", leave, arrive);
                printf("Presionar enter para continuar\n");
                getchar();
            }

        }
}


//ordena passagem por preco na orde crescente
void ordenaPass(DataRoutes *data[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (data[i]->price > data[j]->price) {
               
                DataRoutes *temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}
DataRoutes *selectRoutes(Routes *routes, char *leave, char *arrive, time_t *delimitador){
    NoRoutes *current = routes->ini;

   
    DataRoutes *data[10] = {0};
    int k = 0;
    bool fould = false;
   

    

    
    while(current != NULL){
       
        struct tm tm_leave = {0};  
        time_t leave_time;
        bool stop = false;
       
        if (strptime(current->dataRoutes->dateLeave, "%a %b %d %H:%M:%S %Y", &tm_leave) == NULL) {
            printf("Erro ao converter a data, na função selectRoute\n");
            printf("data:%s",current->dataRoutes->dateLeave);
            perror("ERROR");
            return NULL;
        }

        leave_time = mktime(&tm_leave);

        if(leave_time >= *delimitador){
            if(strcmp(current->dataRoutes->origem, leave ) == 0 && strcmp(current->dataRoutes->destino, arrive) == 0){
                data[k++] = current->dataRoutes;
                fould = true;
            }
        }

        if(fould && (k == 9 || current->next == NULL)){

            while(true){
                ordenaPass(data, k);
                for(int i = 0; i < k; i++){
                    printRoute(data[i]);
                }

                printf("\npressione 10 para sair\tpresione 11 para a proxima pagina\tSelecione a rota desejada entre 0 e %d: ",k);
                int escolha;
                scanf("%d", &escolha);
                while(getchar() != '\n');
                
                if(escolha == 10){
                   
                    return NULL;
                }

                if(escolha == 11){
                    k = 0;
                    for(int i = 0; i < 10; i++){
                        
                        
                    }
                    break;
                }

                if(escolha >= 0 && escolha < k){
                    k = escolha;
                    *delimitador = leave_time;
                    stop = true;
                    break;
                }else{
                    printf("Digite um valor valido\nDigite enter para continuar\n");
                    getchar();
                    clearScreen();
                }
            }
        }

        if(stop){
            break;
        }
        current = current->next;
    }

   
    if(!fould){
        printf("Nenhuma rota encontrada\n");
        return NULL;
    }

    DataRoutes *dataReturn = malloc(sizeof(DataRoutes));
    if(!dataReturn){
        printf("erro ao alocar memória para dataReturn");
        perror("ERROR");
        
        return NULL;
    }

    strcpy(dataReturn->routesTokey, data[k]->routesTokey);
    strcpy(dataReturn->aviaoTokey, data[k]->aviaoTokey);
    strcpy(dataReturn->AviaoEmpresa, data[k]->AviaoEmpresa);
    dataReturn->lugaresMax = data[k]->lugaresMax;
    dataReturn->lugaresDisponiveis = data[k]->lugaresDisponiveis;
    strcpy(dataReturn->origem, data[k]->origem);
    strcpy(dataReturn->destino, data[k]->destino);
    strcpy(dataReturn->dateLeave, data[k]->dateLeave);
    strcpy(dataReturn->dateArrive, data[k]->dateArrive);
    dataReturn->price = data[k]->price;

    return dataReturn;
}


// finaliza a compra de passagem, atualizando o número de passageiros na rota e atualizando a lista de passages
bool buyTicket(Passages *passages, DataRoutes *dataRoute, char *tokeyClient){
    if(!dataRoute){
        printf("dataRoute null\n");
        perror("ERROR");
        return false;
    }
    if(tokeyClient == NULL){
        printf("tokeyClient null\n");
        perror("ERROR");
        return false;
    }

    dataPassages *newPassage = createDataPassages(dataRoute->routesTokey, tokeyClient, dataRoute->price);
    if(!newPassage){
        perror("erro ao criar passagem");
        return false; 
    }

    addPassages(passages, newPassage);

    return true;
}
// busca o usuário na lista de clientes
NoCliente *searchClient(ClienteList *clients, char *tokeyUser){
    NoCliente *current = clients->ini;
    while(current != NULL){
        if(strcmp(current->data->tokeyCliente, tokeyUser) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}
// visualizar passagens compradas pelo usuário
void showALLPassages(ClienteList *clients, char *tokeyUser, Passages *passages, Routes *routes){
    if(passages->size == 0 || tokeyUser == NULL || routes->size == 0){
        printf("Nenhuma passagem encontrada\n");
        return;
    }

    NoCliente *thisClient = searchClient(clients, tokeyUser);
    if(!thisClient){
        printf("Cliente nao encontrado\n");
        return;
    }
    
    clearScreen();
    Passages *thisUserPass = malloc(sizeof(Passages));
    if(!thisUserPass){
        perror("ERROR");
        printf("Erro ao tentar criar lista auxiliar para passageiro!\n");
        return;
    }
    thisUserPass->ini = NULL;
    thisUserPass->end = NULL;
    thisUserPass->size = 0;

    
    NoPassages *currentPass = passages->ini;
    while (currentPass != NULL)
    {
        if(strcmp(currentPass->dataPassages->clientTokey, tokeyUser) == 0){
            addPassages(thisUserPass, currentPass->dataPassages);
        }
        currentPass = currentPass->next;
    }

    if(thisUserPass->size == 0){
        printf("Nenhuma passagem encontrada\n");
        return;
    }

    NoRoutes *currentRoute = routes->ini;
    
    currentPass = thisUserPass->ini;
    while (currentPass != NULL){
        while(currentRoute != NULL){
            if(strcmp(currentPass->dataPassages->routeTokey, currentRoute->dataRoutes->routesTokey) == 0){
                printf("Passageiro:%s\n", thisClient->data->nome);
                printf("Cpf:%s\n", thisClient->data->cpf);
                printf("Origem:%s\n", currentRoute->dataRoutes->origem);
                printf("Destino:%s\n", currentRoute->dataRoutes->destino);
                printf("Data de partida:%s\n", currentRoute->dataRoutes->dateLeave);
                printf("Data de chegada:%s\n", currentRoute->dataRoutes->dateArrive);
                printf("Preco:%f\n", currentRoute->dataRoutes->price);
                printf("Aviao:%s\n", currentRoute->dataRoutes->aviaoTokey);
                printf("Empresa:%s\n", currentRoute->dataRoutes->AviaoEmpresa);
            }
            currentRoute = currentRoute->next;
        }
        currentPass = currentPass->next;
    }
    
    // liberar lista auxiliar de passageiros, sem liberar DataPassages

    currentPass = thisUserPass->ini;
    NoPassages *prev = NULL;

    while(currentPass != NULL){
        prev = currentPass;
        currentPass = currentPass->next;
        free(prev);
    }

    free(thisUserPass);
};
int main(){
    ClienteList *clients = malloc(sizeof(ClienteList));
    if(!clients){
        perror("erro ao alocar memória");
        free(clients);
        exit(1);
    }

    clients->ini = NULL;
    clients->end = NULL;
    clients->size = 0;

    
    char path[] = "DB/clientes.txt";

    fromFileToClientList(clients, path);
    
   
    // login e cadastro

    // Vou usar para armazenar o tokey do usuário logado. será útil na hora da compra
    char *thisClientTokey = NULL;
    
    while(true){
        clearScreen();
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
                char Passwordlogin[50];
                char Cpflogin[15];

                printf("Digite o cpf: ");
                fgets(Cpflogin, 15, stdin);
                while(getchar() != '\n');
                printf("Digite a senha: ");
                fgets(Passwordlogin, 50, stdin);
                while(getchar() != '\n');

                Passwordlogin[strcspn(Passwordlogin, "\n")] = '\0';
                Cpflogin[strcspn(Cpflogin, "\n")] = '\0';
                thisClientTokey = verifyPassword(clients, Cpflogin, Passwordlogin);
                if(thisClientTokey != NULL){
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
                char *tokeyCliete = createClientTokey(clients);

                while(true){
                    printf("Digite o nome: ");
                    fgets(nome, 100, stdin);
                    while(getchar() != '\n' );
                    printf("Digite o cpf: ");
                    fgets(cpf, 12, stdin);
                    
                    if(verifyCpf(clients, cpf) == true){
                        printf("Cpf ja cadastrado\n");
                        continue;
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

                    nome[strcspn(nome, "\n")] = '\0';
                    cpf[strcspn(cpf, "\n")] = '\0';
                    email[strcspn(email, "\n")] = '\0';
                    telefone[strcspn(telefone, "\n")] = '\0';
                    idade[strcspn(idade, "\n")] = '\0';
                    password[strcspn(password, "\n")] = '\0';
                    
                    
                    
                
                    Data *data = createData(tokeyCliete, nome, cpf, email, telefone, idade, password);
                    pushToListClient(clients, data);
                    free(tokeyCliete);    
                    printf("Cadastrado com sucesso\n");
                    fromListToFileClient(clients, path);
                    break;
                }
                

                break;
            case '3':
                char delCpf[12];
                char delPassword[50];
                printf("Digite o cpf: ");
                fgets(delCpf, 12, stdin);
                while(getchar() != '\n');
                printf("Digite a senha: ");
                fgets(delPassword, 50, stdin);
                while(getchar() != '\n');

                delCpf[strcspn(delCpf, "\n")] = '\0';
                delPassword[strcspn(delPassword, "\n")] = '\0';
                
                if(verifyPassword(clients, delCpf, delPassword) == false){
                    printf("Cpf ou senha incorretos\n");
                    continue;
                }

                
                deleteClient(clients, delCpf);
                break;
            case '4':
                printf("Saindo sem salvar\n");
                exit(0);
                break;
            case '5':
                printf("Salvando e saindo\n");
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
    

    // pega as rotas e inicializa a lista de rotas
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

    // pegaa as passagens e inicializa a lista de passagens
    Passages *passages = malloc(sizeof(Passages));
    if(!passages){
        perror("erro ao alocar memória");
        free(passages);
        exit(1);
    }

    passages->ini = NULL;
    passages->end = NULL;
    passages->size = 0;

    char pathPassages[] = "DB/passages.txt";
    fromFileToListPassages(passages, pathPassages);

    const char Estados[][3] = {"AC", "AL", "AP", "AM", "BA", "CE", "DF", "ES", "GO", "MA", "MT", "MS", "MG", "PA", "PB", "PR", "PE", "PI", "RJ", "RN", "RS", "RO", "RR", "SC", "SP", "SE", "TO"};
    while (true){
        bool stop = false;
        char leave[3];
        char arrive[3];
        char op;
        time_t today = time(NULL); // data atual
        bool back = false; // caso seja passagem de volta
        
        clearScreen();
        printf("Escolha uma opcao:\n");
        printf("1 - Comprar ida e volta\n");
        printf("2 - Comprar somente ida\n");
        printf("4 - Listar passagens compradas\n");
        printf("5 - Cancelar compra\n");
        printf("6 - Salvar e sair\n");
        scanf("%c", &op);
        while (getchar() != '\n');

        switch(op){
            case '1':
                printf("Escolha a passagem de ida\nPresione enter para confirmar\n");
                getchar();
                fouldRoute(leave, arrive, Estados, back);
                back = true;
                DataRoutes *ida = selectRoutes(routes, leave, arrive, &today);
                
                if(buyTicket(passages, ida, thisClientTokey)) {
                    printf("Passagem de ida adicionada com sucesso.\n");
                } else {
                    printf("Falha ao adicionar passagem de ida.\n");
                }

                clearScreen();

                
                printf("Escolha a passagem de volta\nPresione enter para confirmar\n");
                getchar();
                fouldRoute(leave, arrive, Estados, back);
                DataRoutes *volta = selectRoutes(routes, arrive, leave, &today);
               
                if(!volta && !(buyTicket(passages, volta, thisClientTokey))){
                    printf("Algum erro ocorreu ao tentar comprar passagem de volta, Tente novamente mais tarde!\n");

                    stop  = true;
                    break;
                }

                
                
                printf("Passagens compradas com sucesso!\n");
                printf("Digite s ou S para comprar novamente ou qualquer outra tecla para finalizar:");
                char breakNow;
                scanf("%c", &breakNow);
                while(getchar() != '\n');
                if(breakNow == 's' || breakNow == 'S'){
                    stop = false;
                }else{
                    printf("\nObrigado pela compra!\n");
                    printf("Salvando passagens e saindo...\n");
                    fromListToFilePassages(passages, pathPassages);
                    fromListToFileRoutes(routes, pathRoutes);

                    
                    stop = true;
                }

                
                
                break;
            case '2':
                printf("Escolha a passagem de ida\nPresione enter para confirmar\n");
                getchar();
                fouldRoute(leave, arrive, Estados, back);
                back = true;
                DataRoutes *TIKET = selectRoutes(routes, leave, arrive, &today);
                
                if(buyTicket(passages, TIKET, thisClientTokey)) {
                    printf("Passagem adicionada com sucesso.\n");
                } else {
                    printf("Falha ao adicionar passagem .\n");
                }

                clearScreen();

                printf("Passagen comprada com sucesso!\n");
                printf("Digite s ou S para comprar novamente ou qualquer outra tecla para finalizar:");
                char breakN;
                scanf("%c", &breakN);
                while(getchar() != '\n');
                if(breakN == 's' || breakN == 'S'){
                    stop = false;
                }else{
                    printf("\nObrigado pela compra!\n");
                    printf("Salvando passagens e saindo...\n");
                    fromListToFilePassages(passages, pathPassages);
                    fromListToFileRoutes(routes, pathRoutes);

                    
                    stop = true;
                }

                break;
            case '4':
                printf("Todas as passagens compradas:\n");
                showALLPassages(clients, thisClientTokey, passages, routes);
                printf("Pressione enter para continuar\n");
                getchar();

                clearScreen();
                
                break;
            case '5':
                exit(0);
                break;
            case '6':
                fromListToFilePassages(passages, pathPassages);
                exit(0);
                break;
            default:
                printf("Opcao invalida\n");
                continue;
                break;

            
        }
        
        if(stop)break;
        
        
        
    }


    
    
 
    return 0;
}
