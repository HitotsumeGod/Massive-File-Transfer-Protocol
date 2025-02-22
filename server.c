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
#include <sys/stat.h>
#include <signal.h>

#define PORT "4444"
#define MAXLEN 1000
#define FLEN 1000000

size_t writeData(FILE *fptr, char *fname, char *buffer, size_t num_bytes);

int main(void) {

	FILE *srcf;
	int serv_sock, cli_sock, errcode;
	struct addrinfo sai, *spai;
	struct sockaddr_storage ss;
	size_t b_read, fin;
	uint32_t got;
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
	if (recv(cli_sock, &got, sizeof(got), 0) == -1)
		perror("recv err");
	fin = (size_t) ntohl(got);
	char *buffer = malloc(fin);
	if (recv(cli_sock, buffer, fin, 0) == -1)
		perror("recv erri");
	b_read = writeData(srcf, "outputf", buffer, fin);
	printf("%s%zu%s\n", "Read ", b_read, " bytes.");
	freeaddrinfo(spai);
	if (close(cli_sock) == -1)
		perror("close err");
	if (close(serv_sock) == -1)
		perror("close err");
	free(buffer);
	return 0;

}

size_t writeData(FILE *srcfile, char *fname, char *buffer, size_t bw) {

	size_t b_read;
	if ((srcfile = fopen(fname, "wb")) == NULL)
		perror("fopen err");
	if ((b_read = fwrite(buffer, sizeof(char), bw, srcfile)) < 0)
		perror("fwrite err");
	if (fclose(srcfile) == -1)
		perror("fclose err");
	if (chmod(fname, 0770) == -1)
		perror("chmod err");
	return b_read;

}
