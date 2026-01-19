#include "../../include/minishell.h"


void	print_tok_list(t_token_lst *lst);

int main (int argc, char **argv)
{
	if (!argc)
		return (0);

	char	*input = argv[1];
	t_token_lst	*tok_lst = lexer(&input);

	print_tok_list(tok_lst);
	free_tok_lst(tok_lst);
}

void	print_tok_list(t_token_lst *lst)
{
	t_token_lst	*current;
	int			i;

	current = lst;
	i = 0;
	printf("\n=== Token List ===\n");
	while (current)
	{
		printf("Token %d:\n", i++);
		printf("  Segment: %s\n", current->segment ? current->segment : "(null)");
		printf("  Mask:    %s\n", current->seg_mask ? current->seg_mask : "(null)");
		printf("  Type:    %d\n", current->type);
		printf("\n");
		current = current->next;
	}
	printf("==================\n");
}
