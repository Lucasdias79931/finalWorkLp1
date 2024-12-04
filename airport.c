#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//Estruturas para os usuários

typedef struct Date{
    char  day[3];
    char  month[3];
    char  year[5];
}Date;

typedef struct Data{
    Date date;
    char name[100];
    char email[100];
    char cpf[12];
    char password[50];
}Data;

typedef struct No{
    Data *data;
    struct No *next;
}No;


typedef struct Users{
    No *ini;
    No *end;
    int size;   
}Users;

Data *creatUser(){
    Data *user = malloc(sizeof(Data));
    if(!user){
        perror("erro ao alocar memória");
        return NULL;
    }

    printf("Digite o seu nome\n");
    fgets(user->name,100,stdin);
    while(getchar() != '\n');

    printf("Digite o seu e-mail\n");
    fgets(user->email,100,stdin);
    while(getchar() != '\n');

    return user;
}


// retorna false se cpf já estiver cadastrada
bool veirfyCpf(char *cpf, Users *user){
    if(user->size == 0){
        return true;
    }

    No *current = user->ini;

    while (current)
    {
        if(strcmp(current->data->cpf, cpf) == 0)return false;
        current = current->next;
    }

    return false;
    

}

// retorna false se password já estiver cadastrada
bool veirfyPass(char *pass, Users *user){
    if(user->size == 0){
        return true;
    }

    No *current = user->ini;

    while (current)
    {
        if(strcmp(current->data->password, pass) == 0)return false;
        current = current->next;
    }

    return false;
    

}



void pushUsers(Users *users, Data *data){
    if(!users){
        perror("Erro em pushUsers");
        return;
    }

    No *new = malloc(sizeof(No));
    if(!new){
        perror("erro de alocação!");
        free(new);
        return;
    }

    new->data = data;

    whiel(true){
        printf("Digite o seu cpf\n");
        fgets(data->cpf,12,stdin);
        while(getchar() != '\n');

        if(!veirfyCpf(data->cpf)){
            printf("cpf já existe!");
            continue;
        }

        break;
       
    }

    while (true){
        printf("Digite a sua senha com até 49 caracteresl\n");
        fgets(data->password,50,stdin);
        while(getchar() != '\n');

        if(!veirfyPass(data->password)){
            printf("senha já existe!");
            continue;
        }

        break;
    }
    



    if(users->end){
        users->end->next = new;
        users->end = new;
    }else{
        users->ini = new;
        users->end = new;
    }
}