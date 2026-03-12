void	debug_print_args(char *path, char **argv)
{
	puts("PATH:");
	puts(path);
	puts("ARGV:");
	debug_puts_many(argv);
}

void	debug_puts_many(char **arr)
{
	if (!arr)
		return ;
	for (int i = 0; arr[i]; i++)
	{
		fputs(arr[i], stdout);
		fputs(" ", stdout);
	}
	puts("");
}
