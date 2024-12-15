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
double savings = 12479.52;
double checking = 1291.22;

char[] checkBalance;
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

        lpTestData = (TestRequest *)(lanRecBuf);
        switch(lpTestData->choice){
          case 1:
            char response[256] = checkBalance(lpTestData->cORs);
            break;
          case 2:
            char response[256] = deposit(lpTestData->cors, lpTestData->amount);
            break;

        printf("Y = %f\n", lpTestData->lfY);
        printf("Text = %s\n", lpTestData->lanText);



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

char[] checkBalance(int cORs){
        char response[256];
     switch(cORs){
         case 1:
            sprintf(response, "Your checking account total is %f\n",checking);
            return reponse;
         case 2:
            sprintf(response, "Your checking account total is %f\n",savings);
            return response;
  }

  char[] deposit(int cORs, int amount){
        char response[256];
        switch(cORs){
          case 1:
            checking+=amount;
            sprintf(response, "Your checking account total is now: %f\n",checking);
            return response;
          case 2:
            savings+=amount;
            sprintf(response, "Your savings account total is now: %f\n",savings);
            return response;
        }
