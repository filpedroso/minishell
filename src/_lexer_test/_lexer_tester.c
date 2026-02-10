#include "../../include/minishell.h"

void	print_tok_list(t_token_lst *lst);

int	main(void)
{
	char		*input;
	t_token_lst	*tok_lst;

	input = get_input_line();
	tok_lst = lexer(&input);
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
		printf("  Segment: %s\n",
			current->segment ? current->segment : "(null)");
		printf("  Mask:    %s\n",
			current->seg_mask ? current->seg_mask : "(null)");
		printf("  Type:    %d\n", current->type);
		printf("\n");
		current = current->next;
	}
	printf("==================\n");
}
