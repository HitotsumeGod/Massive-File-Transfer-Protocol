#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXLEN 100
#define HOST "192.168.0.146"
#define PORT "4444"

int main(void) {

	int sock, errcode;
	size_t got;
	struct addrinfo sai, *spai;
	char buffer[MAXLEN];
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	if ((errcode = getaddrinfo(HOST, PORT, &sai, &spai)) != 0)
		fprintf(stderr, "%s\n", gai_strerror(errcode));
	if ((sock = socket(spai -> ai_family, spai -> ai_socktype, spai -> ai_protocol)) == -1)
		perror("sock err");
	if (connect(sock, spai -> ai_addr, spai -> ai_addrlen) == -1)
		perror("con err");
	if ((got = recv(sock, buffer, sizeof(buffer) - 1, 0)) == -1)
		perror("send err");
	buffer[MAXLEN] = '\0';
	printf("%s%zu%s\n", "Received ", got, " bytes.");
	printf("%s\n", buffer);
	return 0;

}
