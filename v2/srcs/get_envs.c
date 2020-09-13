#include "minishell.h"

t_list		*get_envs(int argc, char **argv, char **envp)
{
	int 	pos;
	t_env	*env;
	t_list	*envs;

	envs = 0;
	if (!argc && argv)
		exit(1);
	while (*envp)
	{
		env = (t_env *)malloc(sizeof(t_env));
		pos = ft_strchr(*envp, '=') - *envp;
		env->key = ft_substr(*envp, 0, pos);
		env->value = ft_substr(*envp, pos + 1, ft_strlen(*envp) - pos - 1);
		ft_lstadd_back(&envs, ft_lstnew(env));
		envp++;
	}
	return (envs);
}