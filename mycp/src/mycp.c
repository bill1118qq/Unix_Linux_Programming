/*
 ============================================================================
 Name        : mycp.c
 Author      : Cao Liang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


#define BUFLEN 512

#define DEBUG
#ifdef DEBUG
#define debug_print(fmt,args...) printf(fmt,##args)
#else
#define debug_print(fmt,args...)
#endif
//#define debug_print(fmt, args...) printf(fmt,##args)

int has_dir_bit(char *dir_name)
{
	struct stat file_stat;

	if (stat(dir_name, &file_stat) < 0)
		return 0;

	if (!S_ISDIR(file_stat.st_mode))
		return 0;

	return 1;
}


int is_dir(char *file_name)
{
	debug_print("file_name:%s\n", file_name);

	return has_dir_bit(file_name);
}

int do_copy(char *source_file, char *target_file)
{
	char buf[BUFLEN];
	int len, wlen;
	int in_fd, out_fd;
	char modified_target_file[255];
	char modified_source_file[255];
	DIR *dir_info;
	struct dirent *dir_entry;

	len = strlen(source_file);
	if ((source_file[len - 1] == '.' && source_file[len - 2] == '/')
			|| (source_file[len - 1] == '.' && source_file[len - 2] == '.' && source_file[len - 3] == '/'))
		return 0;

	if (is_dir(source_file))
	{
		if((dir_info = opendir(source_file)) == NULL)
		{
			perror("open dir failed");
			return -1;
		}

		len = strlen(source_file);
		strcpy(modified_source_file, source_file);
		while((dir_entry = readdir(dir_info)) != NULL)
		{
			if (modified_source_file[len - 1] != '/')
			{
				modified_source_file[len] = '/';
				len++;
			}
			strcpy(modified_source_file + len, dir_entry->d_name);
			do_copy(modified_source_file, target_file);
		}
		return 0;
	}

	 if ((in_fd = open(source_file, O_RDONLY)) < 0)
	{
		printf("open source file failed, source file:%s, error:%d\n", source_file, errno);
		exit(1);
	}

	debug_print("in_fd:%d\n", in_fd);


	strcpy(modified_target_file, target_file);
	if (is_dir(target_file))
	{
		len = strlen(target_file);
		if(modified_target_file[len - 1] != '/')
		{
			modified_target_file[len] = '/';
			len++;
		}
		strcpy(modified_target_file + len, strrchr(source_file, '/'));
	}

	debug_print("target file:%s\n", modified_target_file);

	if ((out_fd = creat(modified_target_file, 0644)) < 0)
	{
		printf("create target file failed, file name:%s, error:%d\n", modified_target_file, errno);
		exit(1);
	}

	while ((len = read(in_fd, buf, BUFLEN)) > 0)
	{
		//debug_print("read line:%s, len:%d\n", buf, len);
		if ((wlen = write(out_fd, buf, len)) != len)
		{
			printf("write error:%d, write len:%d\n", errno, wlen);
			exit(1);
		}
	}
	debug_print("read done, len:%d, buf:%s, BUFlEN:%d\n", len, buf, BUFLEN);

	if (len < 0)
	{
		printf("read error:%d\n", errno);
		exit(1);
	}

	if (close(out_fd) < 0 || close(in_fd) < 0)
	{
		printf("close errno:%d\n", errno);
		exit(1);
	}
}

int main(int argc, char *argv[]) {

	debug_print("cp %s %s\n", argv[1], argv[2]);

	if (argc < 3)
	{
		printf("lack of arguments\n");
		exit(1);
	}

	do_copy(argv[1], argv[2]);

	debug_print("bye\n");
	return 0;

	return EXIT_SUCCESS;
}
