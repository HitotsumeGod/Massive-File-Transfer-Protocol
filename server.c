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

//getdata uses a three step process to prepare a buffer to transport file data: (1) obtain file size in bytes using fseek + ftell, (2) allocate memory equal to size determined in (1) to passed buffer, (3) read target file byte by byte into the buffer. Finally, the prepared buffer is returned.

char *getData(FILE *fptr, char *fname, char *buffer, size_t *fsize);

int main(void) {

	FILE *srcf;
	int serv_sock, cli_sock, errcode, n;
	struct addrinfo sai, *spai;
	struct sockaddr_storage ss;
	char c, *fbuf, *fname;
	size_t b_sent, b_read, n_size, fdata_len;
	uint32_t got, expor;
	socklen_t ss_size;
	ss_size = sizeof(ss);
	memset(&sai, 0, sizeof(sai));
	sai.ai_family = AF_INET;
	sai.ai_socktype = SOCK_STREAM;
	sai.ai_flags = AI_PASSIVE;
	if ((errcode = getaddrinfo(NULL, PORT, &sai, &spai)) != 0) {
		fprintf(stderr, "%s\n", gai_strerror(errcode));
		exit (1);
	}
	if ((serv_sock = socket(spai -> ai_family, spai -> ai_socktype, spai -> ai_protocol)) == -1) {
		perror("sock err");	
		exit(1);
	}
	if (bind(serv_sock, spai -> ai_addr, spai -> ai_addrlen) == -1) {
		perror("bind err");
		exit(1);
	}
	if (listen(serv_sock, 1) == -1) {
		perror("listen err");
		exit(1);
	}
	if ((cli_sock = accept(serv_sock, (struct sockaddr *) &ss, &ss_size)) == -1) {
		perror("accept err");
		exit(1);
	}
	n = 0;
	while (n < 5) {
	if ((b_read = recv(cli_sock, &got, sizeof(got), 0)) == -1) {
		perror("recv err");
		exit(1);
	}
	n_size = (size_t) ntohl(got);
	fname = malloc(n_size);
	if ((b_read = recv(cli_sock, fname, n_size, 0)) == -1) {
		perror("recv err");
		exit(1);
	}
	fbuf = getData(srcf, fname, fbuf, &fdata_len);
	expor = htonl((uint32_t) fdata_len);
	//export fbuf size to allow server to properly allocate memory
	if (send(cli_sock, &expor, sizeof(expor), 0) == -1) {
		perror("send err");
		exit(1);
	}
	if ((b_sent = send(cli_sock, fbuf, fdata_len, 0)) == -1) {
		perror("send err");
		exit(1);
	}
	printf("%s%zu%s\n", "Server sent ", b_sent, " bytes.");
	free(fbuf);
	free(fname);
	n++;
	}
	freeaddrinfo(spai);
	if (close(cli_sock) == -1) {
		perror("close err");
		exit(1);
	}
	if (close(serv_sock) == -1) {
		perror("close err");
		exit(1);
	}
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
