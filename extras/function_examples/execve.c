#include "headers.h"

int	main()
{
	printf("pid: %i\n", (int)getpid());
	char path[] = "/Users/filpedroso/EstudosCS/42/42_projects/common_core/minishell/getcwd";
	puts("calling getcwd:");
	for (int i = 0; i < 16; i++) {
		pid_t pid = fork();
		if (pid == 0)
		{
			if (execve(path, NULL, NULL))
			{
				perror("execve");
				return (1);
			}
		}
		else
		puts("parent here");
	}
}
