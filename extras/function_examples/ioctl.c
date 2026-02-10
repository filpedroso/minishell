#include "headers.h"

int	main(void)
{
	struct winsize	ws;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	printf("Rows: %d, Cols: %d\n", ws.ws_row, ws.ws_col);
}
