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

#define PORT "4444"
#define MAXLEN 1000

int main(int argc, char *argv[]) {

	int serv_sock, cli_sock, errcode;
	struct addrinfo sai, *spai;
	struct sockaddr_storage ss;
	socklen_t ss_size;
	ss_size = sizeof(ss);
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	sai.ai_flags = AI_PASSIVE;
	if ((errcode = getaddrinfo(NULL, PORT, &sai, &spai)) != 0)
		fprintf(stderr, "%s\n", gai_strerror(errcode));
	if ((serv_sock = socket(spai -> ai_family, spai -> ai_socktype, spai -> ai_protocol)) == -1)
		perror("sock err");	
	if (bind(serv_sock, spai -> ai_addr, spai -> ai_addrlen) == -1)
		perror("bind err");
	if (listen(serv_sock, 1) == -1)
		perror("listen err");
	if ((cli_sock = accept(serv_sock, (struct sockaddr *) &ss, &ss_size)) == -1)
		perror("accept err");
	if (send(cli_sock, argv[1], strlen(argv[1]), 0) == -1)
		perror("send err");
	freeaddrinfo(spai);
	if (close(cli_sock) == -1)
		perror("close err");
	if (close(serv_sock) == -1)
		perror("close err");
	return 0;

}
