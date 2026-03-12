#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define SIZE 200

int	main(void)
{
	char	buffer[SIZE];
	char	*cwd;

	cwd = getcwd(buffer, SIZE);
	printf("cwd: %s\n", cwd);
	printf("strlen cwd: %i\n", (int)strlen(cwd));
	if (chdir("/Library/Arturia/Augmented STRINGS/Augmented STRINGS.vst/Contents") ==
		-1)
		perror("chdir");
	puts("chdir ->");
	cwd = getcwd(buffer, SIZE);
	printf("cwd: %s\n", cwd);
	printf("strlen cwd: %i\n", (int)strlen(cwd));
	perror("");
}
