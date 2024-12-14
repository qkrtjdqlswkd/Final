#include "bank.h"
#include <stdio.h>
#include <string.h>



typedef struct {
    double savings_bal;
    double checking_bal;
} BankAccount;


typedef struct bank_func{
    int choice;
    int val;
} BankFunction;



void check_bal(BankAccount *account, BankFunction *choice){
    printf("Which account?\n");
    printf("1) Checking\n");
    printf("2) Savings\n");
    printf("Enter your choice: ");
    scanf("%d", choice->&val);

    switch (choice->val)
        case 1:
            sprintf("Checking Banlace: %.2f\n", account->checking_bal);
            break;
        case 2:
            sprintf("Saving Balance is: %.2f\n", account->savings_bal);
            break;
        default:
            printf("Invalid Selection. \n");
            break;
}


void Deposit_mon(BankAccount *account, BankFunction *choice){
    double amount;
    printf("1) Checking\n");
    printf("2) Savings\n");
    printf("Enter your choice: ");
    scanf("%d", choice->val);


    printf("Where would you like to deposit money to? \n");


}





