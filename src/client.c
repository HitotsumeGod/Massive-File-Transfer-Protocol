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
#define PORT "4444"

char *getData(FILE *fptr, char *fname, char *buffer, size_t *fsize);

int main(int argc, char *argv[]) {

	FILE *srcf;
	int sock, errcode;
	size_t got, fdata;
	uint32_t expor;
	struct addrinfo sai, *spai;
	char *fbuf;
	fbuf = getData(srcf, argv[2], fbuf, &fdata);
	expor = htonl((uint32_t) fdata);
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
	//export fbuf size to allow server to properly allocate memory
	if (send(sock, &expor, sizeof(expor), 0) == -1) {
		perror("send err");
		exit(1);
	}
	if ((got = send(sock, fbuf, fdata, 0)) == -1) {
		perror("send err");
		exit(1);
	}
	printf("%s%zu%s\n", "Sent ", got, " bytes.");
	free(fbuf);
	return 0;

}

char *getData(FILE *srcfile, char *fname, char *buf, size_t *fsize) {

	if ((srcfile = fopen(fname, "rb")) == NULL) {
		perror("fopen err");
		exit(1);
	}
	if (fseek(srcfile, 0L, SEEK_END) == -1) {
		perror("fseek err");
		exit(1);
	}
	if ((*fsize = ftell(srcfile)) == -1) {
		perror("ftell err");
		exit(1);
	}
	if (fseek(srcfile, 0L, SEEK_SET) == -1) {
		perror("rewind err");
		exit(1);
	}
	buf = malloc(*fsize);
	if (fread(buf, sizeof(char), *fsize, srcfile) < 0)
		if (ferror(srcfile)) {
			perror("ferror");
			exit(1);
		} else { 
			perror("eof");
			exit(1);
		}
	return buf;

}
