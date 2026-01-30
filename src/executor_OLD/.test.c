# include "executor.h"

static char	**create_test_tree_single(char *arg, char *arg2);

int main ()
{
	char jessica[] = "jéssica";

	char **args = create_test_tree_single("arvore", jessica);
	puts(args[0]);
	puts(args[1]);
	free(args);
}

static char	**create_test_tree_single(char *arg, char *arg2)
{
	char	**pwd_args;

	pwd_args = (char **)malloc(sizeof(char *) * 3);
	pwd_args[0] = arg;
	pwd_args[1] = arg2;
	pwd_args[2] = NULL;

	return pwd_args;
}
