#include "headers.h"

int main (int argc, char **argv)
{
	/* if (argc != 2)
	{
		puts("usage ./a.out fd");
		return (1);
	}
	int fd = atoi(argv[1]); */
	printf("ttyslot: %i\n", ttyslot());
	for (int fd = 0; fd <= FOPEN_MAX; fd++) {
		if (isatty(fd))
			puts(ttyname(fd));
		else
			perror("isatty");
	}
}
