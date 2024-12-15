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


typedef struct {
    double savings_bal;
    double checking_bal;
} BankAccount;


typedef struct bank_func{
    int choice;
    int val;
    char response[256];
} BankFunction;

