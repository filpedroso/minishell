#include "headers.h"

int	main(void)
{
	char	path[] = "/Users/filpedroso/EstudosCS/42/42_projects/common_core/minishell/getcwd";
	pid_t	pid;

	printf("pid: %i\n", (int)getpid());
	puts("calling getcwd:");
	for (int i = 0; i < 16; i++)
	{
		pid = fork();
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
