/*
 ============================================================================
 Name        : my_more_for_linux.c
 Author      : Cao Liang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define LINE_LEN 512
#define PAGE_LEN 24
void do_more(FILE *filep);
int main(int argc, char *argv[]) {

	FILE *filep;

	//printf("argc = %d\n", argc);

	if (argc == 1)
		do_more(stdin);
	else
	{
		//printf("in else\n");
		while(--argc)
		{
			//printf("argv:%s\n", *++argv);
			if ((filep = fopen(*++argv, "r")) != NULL)
			{
				//printf("file open\n");
				do_more(filep);
				fclose(filep);
			}
			else
			{
				printf("exit\n");
				exit(1);
			}
		}
	}

	return 0;
}


void do_more(FILE *filep)
{
	char line[LINE_LEN];
	int num_of_lines = 0;
	int see_more(), reply;

	printf("start do more\n");
	while(fgets(line, LINE_LEN, filep))
	{
		//printf("line:%s\n", line);
		if (num_of_lines == PAGE_LEN)
		{
			reply = see_more();
			if (reply == 0)
				break;
			num_of_lines -= reply;
		}

		if (fputs(line, stdout) == EOF)
		{
			printf("fputs eof\n");
			exit(1);
		}

		num_of_lines++;
	}

	printf("finish do more\n");
}

int see_more()
{
	int c;
	printf("\033[7m more? \033[m");
	while ((c = getchar()) != EOF)
	{
		if (c == 'q')
			return 0;
		if (c == ' ')
			return PAGE_LEN;
		if (c == '\n')
			return 1;
	}
	return 0;
}
