#include <stdio.h>
#include <stdlib.h>

int main(void) {
    double savBal = 12972.42;
    double checkBal = 942.68;
    int choice;
    int val;
    printf("Welcome to CSE384 Banking!\n");
    while(1) {
        printf("1) Check Balance\n");
        printf("2) Deposit money\n");
        printf("3) Withdraw money\n");
        printf("4) Transfer money\n");
        printf("0) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
            case 1:
                printf("Which account?\n");
                printf("1) Checking\n");
                printf("2) Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &val);
                switch(val)
                    case 1:
                        printf("Checking Balance is: %.2f\n", checkBal);
                        break;
                    case 2:
                        printf("Saving Balance is: %.2f\n", savBal);
                        break;
                break;
            case 2:
                printf("Which account?\n");
                printf("1) Checking\n");
                printf("2) Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &val);
                double deposit;
                printf("How much to deposit?\n Deposit: ");
                scanf("%.2f", &deposit);
                switch(val)
                    case 1:
                        checkBal+=deposit;
                        printf("New Balance is: %.2f\n", checkBal);
                        break;
                    case 2:
                        savBal+=deposit;
                        printf("New Balance is: %.2f\n", savBal);
                        break;
                break;
            case 3:
                printf("Which account?\n");
                printf("1) Checking\n");
                printf("2) Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &val);
                double withdrawl;
                printf("How much to withdraw?\n Withdraw: ");
                scanf("%.2f", &withdrawl);
                switch(val)
                    case 1:
                        if(withdrawl > checkBal) {
                            printf("Insufficient Balance");
                        } else {
                            checkBal-=withdrawl;
                            printf("New Balance is: %.2f\n", checkBal);
                        }
                        break;
                    case 2:
                        if(withdrawl > savBal) {
                            printf("Insufficient Balance");
                        } else {
                            savBal-=withdrawl;
                            printf("New Balance is: %.2f\n", savBal);
                        }
                        break;
                    break;
            case 4:
                printf("From which account are you transferring from?\n");
                printf("1) Checking\n");
                printf("2) Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &val);
                double transfer;
                printf("How much to transfer?\n Transfer: ");
                scanf("%.2f", &transfer);
                switch(val)
                    case 1:
                        if(checkBal < transfer) {
                            printf("Insufficient Balance");
                        } else {
                            checkBal-=transfer;
                            savBal+=transfer;
                            printf("Checking Balance is: %.2f\n", checkBal);
                            printf("Saving Balance is: %.2f\n", savBal);
                        }
                        break;
                    case 2:
                        if(savBal < transfer) {
                            printf("Insufficient Balance");
                        } else {
                            savBal-=transfer;
                            checkBal+=transfer;
                            printf("Checking Balance is: %.2f\n", checkBal);
                            printf("Saving Balance is: %.2f\n", savBal);
                        }
                        break;
                break;
        case 0:
            printf("Exit...\n");
            exit(-1);
    }
    return 0;
}
int checkBalance(int bal) {

}
int deposit(int bal) {

}
int withdrawl(int bal) {

}
int transfer(int to, int from) {

}
