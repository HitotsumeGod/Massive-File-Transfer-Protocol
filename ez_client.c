#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLEN 30
#define PORT "4444"
#define HOST "100.115.92.203"

int sockset(int *sock, struct addrinfo **res);
int handleinc(int *ssock, int *csock, struct addrinfo **res, char *buffer, size_t bufsize);

int main(void) {
	int sock;
	char buffer[MAXLEN];
	struct addrinfo sai, *spai;
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	if (getaddrinfo("127.0.1.1", PORT, &sai, &spai) != 0)
		perror("gai err");
	sockset(&sock, &spai);
	handleinc(&sock, NULL, &spai, buffer, MAXLEN);
	return 0;
}

int sockset(int *sock, struct addrinfo **spai) {
	struct addrinfo *sspai = *spai;
	if ((*sock = socket(sspai -> ai_family, sspai -> ai_socktype, sspai -> ai_protocol)) == -1)
		perror("sock err");
	if (connect(*sock, sspai -> ai_addr, sspai -> ai_addrlen) == -1)
		perror("con err");
	else 
		printf("%s\n", "Connected to remote server.");
	return 0;
}

int handleinc(int *sock, int *nsock, struct addrinfo **spai, char *buffer, size_t bufsize) {
	struct addrinfo *sspai = *spai;
	size_t got;
	if ((got = recv(*sock, buffer, bufsize, 0)) == -1)
		perror("recv err");
	if (snprintf(buffer, sizeof(buffer), "%s", buffer) == -1)
		perror("snprintf err");
	printf("%s\n", buffer);
	return 0;
}
