#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


//Estruturas para os usuários

typedef struct Burthday{
    char  day[3];
    char  month[3];
    char  year[5];
}Burthday;

typedef struct Users{
    Burthday burthday;
    char name[100];
    char email[100];
    char cpf[12];
    char password[50];
     
}Users;

// Verificação de login
Users createUser(){
    char name[100];
    char email[100];
    char day[3];
    char month[3];
    char year[5];
    
    printf("Digite seu nome: ");
    fgets(name, 100, stdin);
    while (getchar() != '\n');

    printf("Digite seu email: ");
    fgets(email, 100, stdin);
    while (getchar() != '\n');

    printf("Digite seu dia de nascimento: ");
    fgets(day, 3, stdin);
    while (getchar() != '\n');

    printf("Digite seu mes de nascimento: ");
    fgets(month, 3, stdin);
    while (getchar() != '\n');

    printf("Digite seu ano de nascimento: ");
    fgets(year, 5, stdin);
    while (getchar() != '\n');

    Users user;
    strcpy(user.name, name);
    strcpy(user.email, email);
    strcpy(user.burthday.day, day);
    strcpy(user.burthday.month, month);
    strcpy(user.burthday.year, year);
    return user;
}
// Retorna false se o cpf já estiver cadastrado
bool verifyCpf(Users *user, int size, char *cpf){
    bool  cpfKey = false, passwordKey = false;

    for(int i = 0; i < size; i++){
        if(strcmp(user[i].cpf, cpf) == 0 ){
            return false;
        }
    }
    return true;

}

// Retorna false se a senha ja estiver cadastrada
bool verifyPassword(Users *user, int size, char *password){
    for(int i = 0; i < size; i++){
        if(strcmp(user[i].password, password) == 0){
            return false;
        }
    }
    return true;
}

void pushUser(Users **Users, int *size){
    if(!Users){
        perror("erro em pushUser");
        exit(1);
    }

    char cpf[12];
    char password[50];

    while (true){
        
        printf("Digite seu CPF: ");
        fgets(cpf, 12, stdin);
        while (getchar() != '\n');
        
        if(verifyCpf(*Users, *size, cpf)){
            break;
        }else{
            printf("CPF ja cadastrado\n");
        }
    }
    
    while (true){
        
        printf("Digite sua senha: ");
        fgets(password, 50, stdin);
        while (getchar() != '\n');
        
        if(verifyPassword(*Users, *size, password)){
            break;
        }else{
            printf("Senha ja cadastrada\n");
        }
    }

    if(size == 0){
        *Users = malloc(sizeof(Users));


    }else{
        
    }      
}




int main(){
    Users *users = NULL;
    int size = 0;
    

    //login
    while (true){
        printf("Escolha uma opcao:\n");
        printf("1 - Cadastrar\n");
        printf("2 - Login\n");
        printf("3 - Sair\n");

        char op;

        scanf("%c", &op);
        while(getchar() != '\n');
        
        switch (op)
        {
        case '1':
            break;
        
        case '2':
            break;
        
        case '3':
            exit(0);
            break;
        default:
            printf("Opcao invalida\n");
            break;
        }
    }
    
}