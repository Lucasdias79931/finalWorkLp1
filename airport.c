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
            return;
        }
        prev = current;
        current = current->next;
    }
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
        fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s", current->data->nome, current->data->cpf, current->data->email, current->data->telefone, current->data->idade, current->data->password);
        current = current->next;
    }
    fclose(file);
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

    while(true){
        printf("Escolha uma opcao:\n");
        printf("1 - logar\n");
        printf("2 - cadastrar\n");
        printf("3 - sair\n");
        bool next = false;
        char op;
        scanf("%c", &op);
        while (getchar() != '\n');
        
        switch(op){
            case '1':
                
                
                break;
            case '2':
                char nome[100];
                char cpf[12];
                char email[100];
                char telefone[12];
                char idade[10];
                char password[50];

                printf("Digite o nome: ");
                fgets(nome, 100, stdin);
                while(getchar() != '\n');
                printf("Digite o cpf: ");
                fgets(cpf, 12, stdin);
                if(verifyCpf(clients, cpf) == true){
                    printf("Cpf ja cadastrado\n");
                    break;
                }

                while(getchar() != '\n');
                printf("Digite o email: ");
                fgets(email, 100, stdin);
                while(getchar() != '\n');
                printf("Digite o telefone: ");
                fgets(telefone, 12, stdin);
                while(getchar() != '\n');
                printf("Digite a idade: ");
                fgets(idade, 10, stdin);
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
                fromListToFileClient(clients, path);
                free(clients);
                exit(0);
            default:
                printf("Opcao invalida\n");
                break;
        }

        if(next)break;
    }
 
    return 0;
}
