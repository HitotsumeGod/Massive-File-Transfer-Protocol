#ifndef _MFTP_H
#define _MFTP_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

typedef struct addrinfo saddrinfo;
typedef struct sockaddr_storage sstorage;

char *getmyipaddr();	//RETRIEVES IP ADDRESS OF FIRST FUNCTIONAL INTERFACE IN HUMAN-READABLE FORMAT
extern int sockserv(int sock, int opts, char *ipaddr, saddrinfo *hints, saddrinfo **res);    //SERVES UP A FUNCTIONAL TCP SOCKET GIVEN AN IP ADDRESS AND USABLE ADDRINFO STRUCTS; RETURNS NULL ON ERROR

#endif // _MFTP_H
