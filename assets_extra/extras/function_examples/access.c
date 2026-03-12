#include <stdio.h>
#include <sys/errno.h>
#include <unistd.h>

int	main(void)
{
	char	path[] = "/Users/filpedroso/EstudosCS/42/42_projects/common_core/minishell/functions.txt";
	int		acc;

	acc = access(path, W_OK);
	printf("acc return: %i\n", acc);
	printf("errno: %i\n", errno);
	perror("perror:");
}
