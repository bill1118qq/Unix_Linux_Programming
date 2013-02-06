#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define debug_print(fmt, args...) printf(fmt,##args)
#define BUF_SIZE 512

int main(int argc, char *argv[])
{
    int idx = 1, read_len;
    int in_fd, out_fd;
    char buf[512];
    if (argc <= 0)
    {
        printf("needs arguments\n");
    }

    debug_print("argc:%d, argv[0]:%s, argv[1]:%s\n", argc, argv[0], argv[1]);
    if (out_fd = open("/dev/tty", O_RDONLY) < 0)
    {
        perror("open /dev/tty error");
    }

    while(idx < argc)
    {
        in_fd = open(argv[idx], O_RDONLY);
        
        while((read_len = read(in_fd, buf, BUF_SIZE)) > 0)
        {
            if (write(out_fd, buf, read_len) < read_len)
            {
                perror("write to tty error");
            }
        }
        idx++;
    }

}
