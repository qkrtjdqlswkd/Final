/*
    Author: Joe Waclawski
    Description: Header file for server code to define
    data common to both the client and server
*/

// this is a simple data structure that allows communication
// of complex data between the client and server
// this header file must be included in both the client and server
// source files.
typedef struct td
{
    int lnX;
    double lfY;
    char lanText[256];
    double sum;
}TestData;
typedef struct tr
{
    int choice;
    int cORs;
    double amount;
    char response[256];
}TestRequest;

typedef struct ba
{
    double checking;
    double savings;

} BankAccount;

