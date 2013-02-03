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

#define BUFLEN 512

#ifdef DEBUG
#define debug_print(fmt,args...) printf(fmt,##args)
#else
#define debug_print(fmt,args...)
#endif
//#define debug_print(fmt, args...) printf(fmt,##args)

int main(int argc, char *argv[]) {
	char buf[BUFLEN];
	int len, wlen;
	int in_fd, out_fd;

	debug_print("cp %s %s\n", argv[1], argv[2]);
	if ((in_fd = open(argv[1], O_RDONLY)) < 0)
	{
		printf("open source file failed, error:%d\n", errno);
		exit(1);
	}

	debug_print("in_fd:%d\n", in_fd);


	if ((out_fd = creat(argv[2], 0644)) < 0)
	{
		printf("create target file failed, error:%d\n", errno);
		exit(1);
	}

	while ((len = read(in_fd, buf, BUFLEN)) > 0)
	{
		debug_print("read line:%s, len:%d\n", buf, len);
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

	debug_print("bye\n");
	return 0;

	return EXIT_SUCCESS;
}
