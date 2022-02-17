#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	//checking existence of parameters
	if(argc < 2) {
		printf("File Name Missing!\n");
		return 1;
	}

	//making sure file opens correctly
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL) {
		printf("UNABLE TO OPEN FILE.\n");
		return 1;
	}

	//iterating through file character by character, using a flag to count words
	char ch;
	int word_count, flag_word = 0;
	while((ch = fgetc(fp)) != EOF) {
		if(ch == ' ' || ch == '\t' || ch ==  '\n' || ch == '\0') {
			if(flag_word == 1) {
				word_count++;
				flag_word = 0;
			}
		}
		else if(flag_word == 0) flag_word = 1;
	}

	//returning total words
	printf("%d total words.\n", word_count);
}
