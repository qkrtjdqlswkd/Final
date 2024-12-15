/*
    Author: Joe Waclawski
    Description: Simple udp server adapted from material found on the internet
    This is the "Server" side of a UDP communication example. It will respond
    to a message from the "Client"
*/
#include<stdio.h>    //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h> 
#include<sys/socket.h>
#include<unistd.h> // close 
#include "socketsetup.h"
#include "bank.h"


#define BUFLEN 512    //Max length of buffer

// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}
    
int main(void)
{

    BankAccount account = {1000.0, 500.0};
    BankFunction bank_func;
    // instantiate our socket structures
    struct sockaddr_in lsSAMe, lsSAOther;

    // instantiate the data structure for communicating
    // with the client
    TestData *lpTestData;
    
    // socket variables
    int lnSocketId = 0; // contains socket handle
    int lnReceiveLen = 0;  // number of bytes received on socket 
    socklen_t lnSockStructLen = sizeof(lsSAOther); // length of socket structure

    // create and clear the recieve buffer
    // not that if messages come in larger then BUFLEN characters, then
    // we will only recieve BUFLEN characters, and the rest will be truncated;
    // which means our data will be corrupted.
    char lanRecBuf[BUFLEN] = {0};
    
    // get the socket setup information
    SocketSetupStruct lsSocketSetup;
    SocketSetup(&lsSocketSetup);
    
    
    //create a UDP socket
    if ((lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        PrintErrorAndExit("socket");
    }
    
    // zero out the structure
    memset((char *) &lsSAMe, 0, sizeof(lsSAMe));
    
    lsSAMe.sin_family = AF_INET; // use IPV4 network addressing
    lsSAMe.sin_port = htons(lsSocketSetup.lnServerPort); // convert to network byte order
    lsSAMe.sin_addr.s_addr = inet_addr(lsSocketSetup.lanServerAddress);
    
    
    //bind socket to our specific port
    if( bind(lnSocketId, (struct sockaddr*)&lsSAMe, sizeof(lsSAMe) ) == -1)
    {
        PrintErrorAndExit("bind");
    }
    
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
        
        //try to receive some data, this is a blocking call
        // -1 indicates an error. anything 0 or above is assumed to be the number of bytes received.
        if ((lnReceiveLen = recvfrom(lnSocketId, lanRecBuf, BUFLEN, 0, 
            (struct sockaddr *) &lsSAOther, &lnSockStructLen)) == -1)
        {
            PrintErrorAndExit("recvfrom()");
        }
        
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(lsSAOther.sin_addr), ntohs(lsSAOther.sin_port));
        
        // cast receieved data to expected type and print data
        lpTestData = (TestData *)(lanRecBuf); 
        printf("X = %d\n", lpTestData->lnX);
        printf("Y = %f\n", lpTestData->lfY);
        printf("Text = %s\n", lpTestData->lanText);


        if (strncmp(lpTestData->lanText, "CHECK_BAL", 9) == 0) {
            check_bal(&account, &bankFunc);  
            strcpy(lpTestData->lanText, bankFunc.response); 
        } else if (strncmp(lpTestData->lanText, "DEPOSIT", 7) == 0) {
            Deposit_mon(&account, &bankFunc);
            strcpy(lpTestData->lanText, bankFunc.response);
        } else if (strncmp(lpTestData->lanText, "WITHDRAW", 8) == 0) {
            withdraw_mon(&account, &bankFunc); 
            strcpy(lpTestData->lanText, bankFunc.response);
        } else if (strncmp(lpTestData->lanText, "TRANSFER", 8) == 0) {
            transfer_mon(&account, &bankFunc);
            strcpy(lpTestData->lanText, bankFunc.response);
        } else {
            strcpy(lpTestData->lanText, "Unknown Command"); 
        }

	char response [1024];


	sprintf(response,"The sum of X + Y is %f\n",lpTestData->lnX + lpTestData->lfY);
	strcpy(lpTestData->lanText,response);
	
        //now reply the client with the same data
        if (sendto(lnSocketId, lanRecBuf, lnReceiveLen, 0, (struct sockaddr*) &lsSAOther, lnSockStructLen) == -1)
        {
            PrintErrorAndExit("sendto()");
        }

        // clear buffer
        memset(lanRecBuf,0,sizeof(lanRecBuf));
    }

    // close the socket resource
    close(lnSocketId);
    return 0;
}




void check_bal(BankAccount *account, BankFunction *choice){
    printf("Which account?\n");
    printf("1) Checking\n");
    printf("2) Savings\n");
    printf("Enter your choice: ");
    scanf("%d", choice->&val);

    switch (choice->val)
        case 1:
            sprintf(choice->response, "Checking Banlace: %.2f\n", account->checking_bal);
            break;
        case 2:
            sprintf(choice->response, "Saving Balance is: %.2f\n", account->savings_bal);
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


    printf("Enter amount: \n");
    scanf("%.2f", &amount);

    switch (choice->val)
        case 1:
            account->checking_bal+=amount;
            sprintf(choice->response, "Deposit successfully\nNew Balance(Checking Account): %.2f\n", account->checking_bal);
            break;
        case 2:
            account->savings_bal+=amount;
            sprintf("Deposit successfully\nNew Balance(Saving Account): %.2f\n", account->savings_bal);
            break;
        default:
            printf("Invalid Selection. \n");
            break;
}


void withdraw_mon(BankAccount *account, BankFunction choice){
    double amount;

    printf("1) Checking\n");
    printf("2) Savings\n");
    printf("Enter your choice: ");
    scanf("%d", choice->val);

    printf("Enter amount: \n");
    scanf("%.2f", &amount);


    switch (choice->val)
        case 1:
            account->checking_bal-=amount;
            sprintf(choice->response, "Deposit successfully\nNew Balance(Checking Account): %.2f\n", account->checking_bal);
            break;
        case 2:
            account->savings_bal-=amount;
            sprintf("Deposit successfully\nNew Balance(Saving Account): %.2f\n", account->savings_bal);
            break;
        default:
            printf("Invalid Selection. \n");
            break;
}


void transfer_mon(BankAccount *account, BankFunction *choice){
    double amount;

    printf("From which account are you transffering from? \n");
    printf("1) Checking\n");
    printf("2) Savings\n");
    printf("Enter your choice: ");
    scanf("%d", choice->val);

    printf("Enter amount: \n");
    scanf("%.2f", &amount);

    switch (choice->val)
    {
        case 1:
            if(amount > account->checking_bal){
                printf("Insufficient Balance.\n");
            }
            else{
                account->checking_bal-=amount;
                account->savings_bal+=amount;
                printf("Transferred Successfully.\n");
                sprintf(choice->response,"Checking Account Balance after transaction: %.2f\n", account->checking_bal);
                sprintf(choice->response, "Saving Account Balance after transaction: %.2f\n", account->savings_bal);
            }
            break;
    }
}











