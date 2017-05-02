#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	char buffer[1024];
	FILE *txt_file = fopen(argv[1], "r");
	struct stat txt_stats;

	if (txt_file == NULL) {
		fprintf(stderr, "error: Unable to open file %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &txt_stats) < 0) {
		fprintf(stderr, "error: Unable to get file stats.\n");
		exit(EXIT_FAILURE);
	}

	printf("FILE SIZE: %d\n", txt_stats.st_size);

	while (fgets(buffer, sizeof(buffer), txt_file)) {
		printf("%s", buffer);
	}

	fclose(txt_file);

	return 0;
}
