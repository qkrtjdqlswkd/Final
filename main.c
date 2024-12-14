#include <stdio.h>

int main(void) {
    double savBal = 12972.42;
    double checkBal = 942.68;
    int choice = 0;
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
                switch(val)
                    case 1:
                        if(withdrawl > checkBal) {
                            printf("Insufficient Balance");
                        } else {

                        }





    }
    return 0;
}
