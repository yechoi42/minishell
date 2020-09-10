#include "minishell.h"

void	catch_sigint()
{
	// ft_putstr_fd("\n?��_?�� ༽つ", 1);
}

void	catch_signals(void)
{
	signal(SIGINT, catch_sigint);
	//should add SIGQUIT func
}

int		find_pipe(char *str)
{
	int	num;

	num = 0;
	while(*str)
	{
		if (*str == '|')
			num++;
		str++;
	}
	return (num);
}

int		find_redir(char *str)
{
	int num;

	num = 0;
	while (*str)
	{
		if (ft_strchr("><", *str))
			num++;
		str++;
	}
	return (num);
}

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	t_list	*cmds;
	t_list 	*envs;

	g_envp = envp;
	line = 0;
	cmds = 0;
	envs = get_envs(argc, argv, envp);
	catch_signals();
	show_art();
	while (1)
	{
		show_prompt(envs, &line);
	 	if ((cmds = get_cmds(line)) == NULL)
			continue;
	 	exec_cmds(cmds, envs);
		init_cmds(&cmds);
	}
	return (0);
}