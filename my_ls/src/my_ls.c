/*
 ============================================================================
 Name        : my_ls.c
 Author      : Cao Liang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 4096

//#define DEBUG
#ifdef DEBUG
#define debug_print(fmt,args...) printf(fmt,##args)
#else
#define debug_print(fmt,args...)
#endif
char *uid_to_name(uid_t uid)
{
	static char name_str[10];
	struct passwd *pwd;

	if ((pwd = getpwuid(uid)) == NULL)
	{
		sprintf(name_str, "%d", uid);
		return name_str;
	}
	else
		return pwd->pw_name;
}

char *gid_to_name(gid_t gid)
{
	static char name_str[10];
	static struct group *out_group;

	if ((out_group = getgrgid(gid)) == NULL)
	{
		sprintf(name_str, "%d", gid);
		return name_str;
	}
	else
		return out_group->gr_name;
}

void mode_to_letters(mode_t mode, char *mode_str)
{
	if (S_ISDIR(mode)) mode_str[0] = 'd';
	if (S_ISCHR(mode)) mode_str[0] = 'c';
	if (S_ISBLK(mode)) mode_str[0] = 'b';
	if (S_ISREG(mode)) mode_str[0] = '-';

	if (mode & S_IRUSR) mode_str[1] = 'r';
	else mode_str[1] = '-';
	if (mode & S_IWUSR) mode_str[2] = 'w';
	else mode_str[2] = '-';
	if (mode & S_IXUSR) mode_str[3] = 'x';
	else mode_str[3] = '-';

	if (mode & S_IRGRP) mode_str[4] = 'r';
	else mode_str[4] = '-';
	if (mode & S_IWGRP) mode_str[5] = 'w';
	else mode_str[5] = '-';
	if (mode & S_IXGRP) mode_str[6] = 'x';
	else mode_str[6] = '-';

	if (mode & S_IROTH) mode_str[7] = 'r';
	else mode_str[7] = '-';
	if (mode & S_IWOTH) mode_str[8] = 'w';
	else mode_str[8] = '-';
	if (mode & S_IXOTH) mode_str[9] = 'x';
	else mode_str[9] = '-';

	mode_str[10] = '\0';

	return;
}

void show_info(char *file_name, struct stat *stat_info)
{
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void	mode_to_letters();
	char    modestr[11];


	debug_print("file_name:%s\n", file_name);
	mode_to_letters( stat_info->st_mode, modestr );

	printf( "%s"    , modestr );
	printf( "%4d "  , (int) stat_info->st_nlink);
	printf( "%-8s " , uid_to_name(stat_info->st_uid) );
	printf( "%-8s " , gid_to_name(stat_info->st_gid) );
	printf( "%8ld " , (long)stat_info->st_size);
	printf( "%.12s ", 4+ctime(&stat_info->st_mtime));
	printf( "%s\n"  , file_name );

}

void do_stat(char *file_name)
{
	struct stat file_info;

	if (stat(file_name, &file_info) < 0)
	{
		fprintf(stderr, "get file:%s stat error:%d\n", file_name, errno);
		return;
	}
	else
	{
		debug_print("file_name:%s\n", file_name);
		show_info(strrchr(file_name, '/') + 1, &file_info);
	}
}

static int do_compare(const void *str1, const void *str2)
{
	return strcmp(* (char * const *) str1, * (char * const *)str2);
}

void do_ls(char *dir_name)
{
	struct dirent *direntry;
	DIR *dir = opendir(dir_name);
	char *file_array[BUF_SIZE];
	char full_path_and_name[512];
	int idx = 0, i = 0;

	memset(file_array, 0, sizeof(char *) * BUF_SIZE);

	if (dir == NULL)
		return;

	while((direntry = readdir(dir)) != NULL)
	{
		file_array[idx]= malloc(strlen(direntry->d_name) + 1);
		strcpy(file_array[idx], direntry->d_name);
		file_array[idx][strlen(direntry->d_name)] = '\0';
		debug_print("file name:%s\n", file_array[idx]);
		idx++;
	}

	debug_print("starting to qsort ========================================\n");
	debug_print("size:%d\n", sizeof(file_array[0]));
	qsort(&file_array[0], idx, sizeof(file_array[0]), do_compare);

	for (i = 0; i < idx; i++)
		debug_print("array name:%s\n", file_array[i]);

	debug_print("qsort done ==============================================\n");

	strcpy(full_path_and_name, dir_name);
	for (i = 0; i < idx; i++)
	{
		full_path_and_name[strlen(dir_name)] = '\0';
		strcat(full_path_and_name, file_array[i]);
		debug_print("full name:%s\n", full_path_and_name);
		do_stat(full_path_and_name);
	}

	closedir(dir);

	return;

}

int main(int argc, char *argv[]) {

	if (argc == 1)
		do_ls(".");
	else
		do_ls(argv[1]);

	return EXIT_SUCCESS;
}
