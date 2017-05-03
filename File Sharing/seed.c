#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

FILE *open_split_file(int split_count);
void split_file(int argc, char **argv);
void merge_files(int user_count);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "error: Missing parameters.\n");
		exit(EXIT_FAILURE);
	}

	system("exec rm -r ./tmp/*");
	split_file(argc, argv);
	merge_files(3);

	return 0;
}

FILE *open_split_file (int split_count)
{
	char filename[256];	
	snprintf(filename, sizeof(filename), 
			 "./tmp/split_%d.txt", split_count);

	FILE *new_split_file = fopen(filename, "w+");

	if (new_split_file == NULL) {
		fprintf(stderr, "error: Unable to open split file.\n");
		exit(EXIT_FAILURE);
	}

	return new_split_file;
}

void split_file (int argc, char **argv) 
{
	char ch;
	FILE *txt_file = fopen(argv[1], "r");
	FILE *split_file;
	struct stat txt_stats;
	int leechers = 3;
	int file_size = 0;
	int split_size;
	int split_count = 0;

	if (txt_file == NULL) {
		fprintf(stderr, "error: Unable to open file %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &txt_stats) < 0) {
		fprintf(stderr, "error: Unable to get file stats.\n");
		exit(EXIT_FAILURE);
	}

	printf("FILE SIZE: %d\n", txt_stats.st_size);
	//TODO change (leechers+1) to accomodate for real leechers
	split_size = ceil(txt_stats.st_size / leechers);
	split_file = open_split_file(split_count++);

	while ((ch = fgetc(txt_file)) != EOF) {
		fputc(ch, split_file);
		file_size++;

		if (file_size > split_size) {
			file_size = 0;
			fclose(split_file);
			split_file = open_split_file(split_count++);
		}
	}

	fclose(split_file);
	fclose(txt_file);
}

void merge_files (int user_count)
{
	int split_count = 0;
	char ch;
	char filename[256];
	FILE *merge_file = fopen("./tmp/merge.txt", "w+");
	FILE *txt_file;

	if (merge_file == NULL) {
		fprintf(stderr, "error: Unable to open merge file.\n");
		exit(EXIT_FAILURE);
	}

	for (; split_count < user_count; split_count++) {
		snprintf(filename, sizeof(filename), 
				 "./tmp/split_%d.txt", split_count);

		txt_file = fopen(filename, "r");

		if (txt_file == NULL) {
			fprintf(stderr, "error: Unable to open split file.\n");
			exit(EXIT_FAILURE);
		}

		while ((ch = fgetc(txt_file)) != EOF) {
			fputc(ch, merge_file);
		}

		fclose(txt_file);
	}

	fclose(merge_file);
}
