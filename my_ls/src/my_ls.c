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

#define debug_print(fmt,args...) printf(fmt,##args)

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
		fprintf(stderr, "get file:%s stat error\n", file_name);
		return;
	}
	else
	{
		show_info(file_name, &file_info);
	}
}

void do_ls(char *dir_name)
{
	struct dirent *direntry;
	DIR *dir = opendir(dir_name);

	if (dir == NULL)
		return;

	while((direntry = readdir(dir)) != NULL)
	{
		do_stat(direntry->d_name);
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
