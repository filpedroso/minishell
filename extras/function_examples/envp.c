#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>

int main (int argc, char **argv, char **envp)
{
	for (int i = 0; envp[i]; i++) {
			printf("%i) ", i);
			puts(envp[i]);
		}
}
