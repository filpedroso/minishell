#include "headers.h"

void print_dirent_fields(const struct dirent *entry);

int main ()
{
	char	path[] = "..";

	DIR	*directory = opendir(path);
	struct dirent	*dirent;
	while ( (dirent = readdir(directory)) )
	{
		print_dirent_fields(dirent);
		putchar('\n');
	}
}

void print_dirent_fields(const struct dirent *entry)
{
    printf("d_ino:		%lu\n",	(unsigned long)entry->d_ino);
    printf("d_reclen:	%u\n",	(unsigned int)entry->d_reclen);
    printf("d_type:		%u\n",	(unsigned int)entry->d_type);
    printf("d_namlen:	%u\n",	(unsigned int)entry->d_namlen);
    printf("d_name:		%s\n",	entry->d_name);
}
