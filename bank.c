/*
    Author: Adam Rogers, Sungbin Park
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

double checking = 1283.34;
double savings = 10000.00;

char* checkBalance(int cORs);
char* deposit(int cORs, double amount);
char* withdrawl(int cORs, double amount);
char* transfer(int cORs, double amount);
// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}
    
int main(void)
{

    // instantiate our socket structures
    struct sockaddr_in lsSAMe, lsSAOther;

    // instantiate the data structure for communicating
    // with the client
    TestRequest *lpTestData;
    
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
        char response[256];

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
        //LOGIC, OUR LOGIC
        lpTestData = (TestRequest *)(lanRecBuf);
        switch(lpTestData->choice) {
            case 1:
                sprintf(response, "%s", checkBalance(lpTestData->cORs));
                break;
            case 2:
                sprintf(response, "%s", deposit(lpTestData->cORs, lpTestData->amount));
                break;
            case 3:
                sprintf(response, "%s", withdrawl(lpTestData->cORs, lpTestData->amount));
                break;
            case 4:
                sprintf(response, "%s", transfer(lpTestData->cORs, lpTestData->amount));
                break;
            case 0:
                sprintf(response, "Thank you for using CSE384 Bank, goodbye!");
                if (sendto(lnSocketId, lanRecBuf, lnReceiveLen, 0, (struct sockaddr*) &lsSAOther, lnSockStructLen) == -1)
                {
                    PrintErrorAndExit("sendto()");
                }
                memset(lanRecBuf,0,sizeof(lanRecBuf));
                close(lnSocketId);
                return 0;
        }
        strcpy(lpTestData->response, response);
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

char* checkBalance(int cORs) {
    static char response[256];
    switch(cORs){
        case 1:
            sprintf(response, "Your checking account total is %.2f\n",checking);
            break;
        case 2:
            sprintf(response, "Your checking account total is %.2f\n",savings);
            break;
    }
    return response;
}

    char* deposit(int cORs, double amount) {
        static char response[256];
        switch(cORs) {
            case 1:
                printf("Before deposit: %.2f, Deposit amount: %.2f\n", checking, amount);
                checking+=amount;
            sprintf(response, "Your checking account total is now: %.2f\n",checking);
            break;
            case 2:
                savings+=amount;
            sprintf(response, "Your savings account total is now: %.2f\n",savings);
            break;
        }
        return response;
    }
    char* withdrawl(int cORs, double amount){
        static char response[256];
        switch(cORs) {
            case 1:
                if(amount>checking) {
                    sprintf(response, "Insufficient Balance");
                    break;
                }else {
                    checking-=amount;
                    sprintf(response,"Your new checking account balance is: %f\n",checking);
                    break;
                }
            case 2:
                if(amount>savings) {
                    sprintf(response, "Insufficient Balance");
                    break;
                }else {
                    savings-=amount;
                    sprintf(response,"Your new savings account balance is: %f\n",savings);
                    break;
                }
        }
        return response;
    }
    char* transfer(int cORs, double amount) {
        static char response[256];
        switch(cORs) {
            case 1:
                if(amount>checking) {
                    sprintf(response, "Insufficient Balance");
                }else {
                    printf("checking before: %.2lf\namount: %.2lf\n", checking, amount);
                    checking -= amount;
                    printf("checking after: %.2lf\n", checking);
                    printf("savings before: %.2lf\namount: %.2lf\n", savings, amount);
                    savings += amount;
                    printf("savings after: %.2lf\n", savings);
                    sprintf(response,"Checking Balance: %lf\nSavings Balance: %lf\n",checking, savings);
                }
                break;
            case 2:
                if(amount>savings) {
                    sprintf(response, "Insufficient Balance");
                } else {
                    savings-=amount;
                    checking+=amount;
                    sprintf(response,"Checking Balance: %lf\nSavings Balance: %lf\n", checking, savings);
                }
                break;
        }
        return response;
    }