/*
    Author: Joe Waclawski
    Description: Simple udp client adapted from material found on the internet
    This is the "Client" side of a UDP communication example. It will prompt
    the user for a message, and send it to the server. It will then  display
    the server's response. 

    There is a recieve thread that is waiting for incomming data
*/

#include<stdio.h>    //printf 
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h> //close
#include "socketsetup.h"
#include "bank.h"
#include "pthread.h"

#define BUFLEN 512    //Max length of buffer

// function to truncate stdin (i.e. keyboard input)
// unused at this time. I just left it here in case I need it in the future
void FlushStdin(void);


// socket structure to define properties
// must be at file scope so the thread function can see it as well as main
struct sockaddr_in lsSAOther;
socklen_t lnSockStructLen=sizeof(lsSAOther);    

// place to store socket handle.
// must be at file scope so the thread function can see it as well as main
int lnSocketId = 0;



// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}

int main()
{

    // please to hold received data
    char lanRecBuf[1012];
    
    // get the socket setup information
    SocketSetupStruct lsSocketSetup;
    SocketSetup(&lsSocketSetup);

    // instantiate data structure for comminucating with server
    TestRequest lsTestData;

    // attempt to create a socket
    if ( (lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        PrintErrorAndExit("socket");
    }

    // zero out the structure used to define where the 
    // data is going (destination) on the socket 
    memset((char *) &lsSAOther, 0, sizeof(lsSAOther));
    
    // initialize the destination properties
    lsSAOther.sin_family = AF_INET;  // IPV4
    lsSAOther.sin_port = htons(lsSocketSetup.lnServerPort); // convert port to network byte order
    lsSAOther.sin_addr.s_addr = inet_addr(lsSocketSetup.lanServerAddress);
    
    // set the destination address in the destination properties
    if (inet_aton(lsSocketSetup.lanServerAddress, &lsSAOther.sin_addr) == 0) 
    {
        PrintErrorAndExit("inet_aton() failed\n");
    }
    printf("Welcome to CSE384 Banking!\n");
    while(1)
    {
        printf("1) Check Balance\n");
        printf("2) Deposit money\n");
        printf("3) Withdraw money\n");
        printf("4) Transfer money\n");
        printf("0) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &lsTestData.choice);
        switch(lsTestData.choice){
            case 1:
                printf("Which account?\n1)Checking\n2)Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &lsTestData.cORs);
                break;
            case 2:
                printf("Which account?\n1)Checking\n2)Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &lsTestData.cORs);
                printf("Please enter deposit: ");
                scanf("%f", &lsTestData.amount);
                break;
            case 3:
                printf("Which account?\n1)Checking\n2)Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &lsTestData.cORs);
                printf("Please enter withdrawl amount: ");
                scanf("%f", &lsTestData.amount);
                break;
            case 4:
                printf("Which account would you like to transfer from?\n1)Checking\n2)Savings\n");
                printf("Enter your choice: ");
                scanf("%d", &lsTestData.cORs);
                printf("Please enter transfer amount: ");
                scanf("%f", &lsTestData.amount);
                break;
            case 0:
                printf("Thank you for choosing CSE384 bank. Goodbye!");
                exit(-1);

        }

        printf("Processing Request\n");
	// flush stdout so the above message is printed immediately
        fflush(stdout);

        //send the Message
        if (sendto(lnSocketId, (char *)&lsTestData, sizeof(lsTestData) , 0 , 
            (struct sockaddr *) &lsSAOther, lnSockStructLen)==-1)
        {
            PrintErrorAndExit("sendto()");
        }

        //try to receive some data, this is a blocking call
        if (recvfrom(lnSocketId, lanRecBuf, BUFLEN, 0,
            (struct sockaddr *) &lsSAOther, &lnSockStructLen) == -1)
        {
            PrintErrorAndExit("recvfrom()");
        }



	// print information back from server
	TestRequest *returnData = (TestRequest*)(lanRecBuf);
	
	printf("%s\n",returnData->response);

    }
    // close our socket resource
    close(lnSocketId);
    return 0;
}

void FlushStdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
	printf("%c",c);
    }
}
