#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

typedef struct s_env 
{
	char *key;
	char *value;
} t_env;

t_list	*get_envs(int argc, char **argv, char **envp)
{
	t_list	*envs;
	t_env	*env;
	int 	pos;
	
	if (!argc && argv)
		exit(1);
	while (*envp)
	{
		env = (t_env *)malloc(sizeof(t_env));
		pos = ft_strchr(*envp, '=') - *envp;
		env->key = ft_substr(*envp, 0, pos - 1);
		env->value = ft_substr(*envp, pos + 1, ft_strlen(*envp) - pos - 1);
		ft_lstadd_back(&envs, ft_lstnew(env));
		envp++;
	}
	return (envs);
}

void	catch_sigint()
{
	ft_putstr_fd("\n◕_◕ ༽つ", 1);
}

void	catch_signals(void)
{
	signal(SIGINT, catch_sigint);
	//should add SIGQUIT func
}

int		main(int argc, char **argv, char **envp)
{
	t_list *envs;
	char	*line;

	line = 0;
	envs = get_envs(argc, argv, envp);
	catch_signals();
	while (1)
	{
		ft_putstr_fd("◕_◕ ༽つ", 1);
		get_next_line(0, &line);
	}
	return (0);
}

