/*
Due Date:       April 5th, 2021
Creators:       Casey Rock & Malachi Parks
Section:        CISC450-010
Assignment:     Programming Assignment 1

File Description: Used as the .h file in the TCPClient.c and TCPServer.c file instead of all the seperate header files.
 Also allows the change of MESSMAX and PORT easily rather than hardcoding values into the .c files
*/

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define MESSMAX 80	//Max length of a message including newline char
#define MAX_FILE_NAME 100
#define PORT 65211	//Port used for source port on client
#define SERVER_LISTEN_QUEUE 10
 
char serverIPV4[] = "192.139.12.3";
char clientIPV4[] = "192.139.12.4" ;

struct packet {
        short count;
        short pktSequenceNumber ;
        char data[MESSMAX];
} ;
