#include "minishell.h"

void 	handle_signal(int signo)
{
	int		status;

	if (signo == SIGINT)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
		ft_putstr_fd("\n", 1);
		show_prompt(g_env_user);
	}
	else if (signo == SIGQUIT)
	{
		signo = wait(&status);
		g_exit_value = status / 256;
		ft_putstr_fd("\b\b  \b\b", 1);
		if (signo != -1)
			ft_putstr_fd("^\\Quit: 3\n", 1);
		else
			return ;
	}
	return ;
}