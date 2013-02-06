/*
 ============================================================================
 Name        : my_who.c
 Author      : Cao Liang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>
#include <time.h>


#define debug_print(fmt,args...) printf(fmt,##args)
#define NUM_REC 16
#define UTMP_SIZE (sizeof(struct utmp))

char utmp_buf[UTMP_SIZE * NUM_REC];
int num_rec = 0;
int utmp_fd = -1;
int max_rec = 0;

void show_time(time_t timeval)
{
	printf("%12.12s", ctime(&timeval) + 4);
}

int show_info(struct utmp *info)
{
	//if (info->ut_type != USER_PROCESS)
	//	return 0;
	printf("%-8.8s", info->ut_name);
	printf(" ");
	printf("%-8.8s", info->ut_line);
	printf(" ");
	printf("%10ld", info->ut_time);
	printf(" ");
	printf("(%s)", info->ut_host);
	show_time(info->ut_time);
	printf("\n");
	return 0;
}

int utmp_open()
{
	utmp_fd = open(WTMP_FILE, O_RDONLY);
	num_rec = 0;
	max_rec = 0;

	return utmp_fd;
}

int reload_utmp()
{
	int len = read(utmp_fd, utmp_buf, UTMP_SIZE * NUM_REC);
	if (len < 0)
		return 0;

	num_rec = 0;
	max_rec = len / UTMP_SIZE;

	return max_rec;

}

struct utmp *next_utmp()
{
	struct utmp *utmp_rec;

	if (utmp_fd == -1)
		return NULL;

	if (num_rec == max_rec && reload_utmp() == 0)
		return NULL;

	utmp_rec = (struct utmp*)&utmp_buf[UTMP_SIZE * num_rec];
	num_rec++;
    
    if (utmp_rec->ut_type != USER_PROCESS && strcmp(utmp_rec->ut_user, "reboot") 
        && strcmp(utmp_rec->ut_user, "shutdown"))
        return next_utmp();

	return utmp_rec;

}

void utmp_close()
{
	if (utmp_fd != -1)
		close(utmp_fd);
}

int main(void) {
	struct utmp *p_utmp_info;

	if (utmp_open() < 0)
		exit(1);



	while (p_utmp_info = next_utmp())
	{
		show_info(p_utmp_info);
	}

	utmp_close();


	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
