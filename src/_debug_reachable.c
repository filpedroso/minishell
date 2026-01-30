// Checks if end is sane, from the same list
void	debug_reachable(t_token_lst *start, t_token_lst *end)
{
	for (t_token_lst *cur = start; cur; cur = cur->next) {
		if (cur == end)
			return (ft_putstr_fd("REACHABLE\n", 2));
	}
	return (ft_putstr_fd("UNREACHABLE\n", 2));
}