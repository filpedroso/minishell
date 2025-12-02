#include "headers.h"

int main ()
{
	char path[] = "/Users/filpedroso/EstudosCS/42/42_projects/common_core/minishell/redir_stdout";
	int fd = open(path, O_WRONLY);
	puts("dupping stout...");
	dup2(fd, 1);
	puts("test OK: file received a string from puts(), which prints to stdout. Crazy stuff!");
}
