#include "minishell.h"

void	init_redir(char *command, t_redir *r)
{
	int num;

	num = 0;
	while (*command)
	{
		if (ft_strchr("><", *command))
			num++;
		command++;
	}
	r->argc = num + 1;
	r->argv = (char **)malloc(sizeof(char *) * (r->argc + 1));
	r->types = (char *)malloc(sizeof(char) * (r->argc + 1));
	r->cmds = NULL;
	r->file_name = 0;
}

int		is_single_redir(char *command, int i)
{
	int ret;

	if (command[i] == '>' && command[i - 1] != '>' && command[i + 1] != '>')
		return (1);
	return (0);
}

int		find_redir_type(char *command, int i)
{
	if (command[i] == '>' && command[i + 1] == '>')
		return (DREDIR);
	else if (is_single_redir(command, i)) 
		return (REDIR);
	return (BREDIR);
}

int		has_syntax_error(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_strchr(" \n", str[i]))
			return (0);
		i++;
	}
	return (1);
}