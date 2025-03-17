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

#define MAXLEN 100
#define PORT "4444"

size_t writeData(FILE *srcfile, char *fname, char *buffer, size_t bw);

int main(int argc, char *argv[]) {

	FILE *srcf;
	char *fbuf, *fname;
	int sock, errcode;
	size_t b_read, b_sent, n_size, fin, temp;
	uint32_t expor, got;
	struct addrinfo sai, *spai;
	if (argc != 2) {
		printf("%s\n", "Improper format. To run, use './client <host IP>'.");
		exit (1);
	}
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	if ((errcode = getaddrinfo(argv[1], PORT, &sai, &spai)) != 0) {
		fprintf(stderr, "%s\n", gai_strerror(errcode));
		exit(1);
	}
	if ((sock = socket(spai -> ai_family, spai -> ai_socktype, spai -> ai_protocol)) == -1) {
		perror("sock err");
		exit(1);
	}
	if (connect(sock, spai -> ai_addr, spai -> ai_addrlen) == -1) {
		perror("con err");
		exit(1);
	}
	while (1) {
	fname = NULL;
	if (getline(&fname, &temp, stdin) == -1) {
		perror("getline err");
		exit(1);
	}
	if (strcmp(fname, "exit\n") == 0)
		exit(1);
	n_size = strlen(fname) - 1;
	expor = htonl((uint32_t) n_size);
	if ((b_sent = send(sock, &expor, sizeof(expor), 0)) == -1) {
		perror("send err");
		exit(1);
	}
	if ((b_sent = send(sock, fname, n_size, 0)) == -1) {
		perror("send err");
		exit(1);
	}
	if ((b_read = recv(sock, &got, sizeof(got), 0)) == -1) {
		perror("recv err");
		exit(1);
	}
	fin = (size_t) ntohl(got);
	fbuf = malloc(fin);
	if (recv(sock, fbuf, fin, 0) == -1) {
		perror("recv erri");
		exit(1);
	}
	printf("%s%zu%s\n", "Client read ", fin, " bytes.");
	b_read = writeData(srcf, "outputf", fbuf, fin);
	free(fname);
	free(fbuf);
	n++;
	}
	freeaddrinfo(spai);
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
