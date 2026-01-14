#include "project.h"

int	main(void)
{
	t_proj	*proj;

	proj = init_main_struct();
	run(proj);
	cleanup(proj);
	return (SUCCESS_EXIT);
}
