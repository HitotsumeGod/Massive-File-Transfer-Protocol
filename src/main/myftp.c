#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

	FILE *this, *that;
	char *buffer;
	long bufit;
	if ((this = fopen(argv[1], "rb")) == NULL)
		perror("fopen err");
	if ((that = fopen(argv[2], "wb")) == NULL)
		perror("fopen err");
	if (fseek(this, 0L, SEEK_END) == -1)
		perror("fseek err");
	if ((bufit = ftell(this)) == -1)
		perror("ftell err");
	if (fseek(this, 0L, SEEK_SET) == -1)
		perror("rewind err");
	buffer = malloc(bufit);
	if (fread(buffer, sizeof(char), bufit, this) < 0)
		if (ferror(this))
			perror("ferror");
		else 
			perror("eof");
	if (fwrite(buffer, sizeof(char), bufit, that) < 0)
		perror("fwrite rr");
	free(buffer);
	printf("%lu%s\n", (8 * bufit), " bits read and written.");
	if (chmod(argv[2], 0770) == -1)
		perror("chmod err");
	if (fclose(this) == -1)
		perror("fclose err");
	if (fclose(that) == -1)
		perror("fclose err");
	return 0;

}
