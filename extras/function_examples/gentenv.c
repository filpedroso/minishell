#include "headers.h"

int main (int argc, char **argv)
{
	char	*env_var = argv[1];
	
	puts(getenv(env_var));
}
