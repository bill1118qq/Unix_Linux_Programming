#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	char *current_id = getlogin();
	if (current_id)
		printf("%s\n", current_id);

    return 0;	
}

