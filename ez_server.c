#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXLEN 30
#define PORT "4444"
#define HOST "0.0.0.0"

int sockset(int *sock, struct addrinfo **res);	//create, bind, listen
int handleinc(int *ssock, int *csock, struct sockaddr_storage *box, socklen_t *boxthick, char *buffer, size_t bufsize);

int main(void) {
	int serv_sock, cli_sock;
	struct addrinfo sai, *spai;
	struct sockaddr_storage ss;
	socklen_t ss_size = sizeof(ss);
	char *buffer = "Hi Client!!";
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(NULL, PORT,  &sai, &spai) != 0)
		perror("gai err");
	if (sockset(&serv_sock, &spai) != 0)
		perror("ss err");
	if (handleinc(&serv_sock, &cli_sock, &ss, &ss_size, buffer, (size_t) MAXLEN) != 0)
		perror("handler err");
	close(serv_sock);
	close(cli_sock);
	return 0;
}

int sockset(int *ss, struct addrinfo **spai) {
	struct addrinfo *sspai = *spai;
	if ((*ss = socket(sspai -> ai_family, sspai -> ai_socktype, sspai -> ai_protocol)) == -1)
		perror("socket err");
	if (bind(*ss, sspai -> ai_addr, sspai -> ai_addrlen) == -1)
		perror("bind err");
	if (listen(*ss, 1) == -1)
		perror("listen err");
	return 0;
}

int handleinc(int *serv_sock, int *cli_sock, struct sockaddr_storage *ss, socklen_t *ss_len, char *buffer, size_t bufsize) {
	size_t sent;
	if ((*cli_sock = accept(*serv_sock, (struct sockaddr*) ss, ss_len)) == -1)
		perror("accept err");
	if ((sent = send(*cli_sock, buffer, bufsize, 0)) == -1)
		perror("send err");
	return 0;
}
