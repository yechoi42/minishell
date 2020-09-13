#include "minishell.h"

int			is_valid_env(char *arg)
{
	if (ft_strchr(arg, '='))
		return (1);
	return (0);
}

int			is_exist_key(char *key, t_list *envs)
{
	if (ft_strncmp(((t_env *)envs->content)->key, key, ft_strlen(key)) == 0)
		return (1);
	return (0);
}


char		*find_value(char *key, t_list *envs)
{
	while (envs)
	{
		if (is_exist_key(key, envs))
			return (((t_env *)envs->content)->value);
		envs = envs->next;
	}
	return ("");
}

char		*find_path(char *argv, t_list *envs)
{
	int			idx;
	char		*temp;
	char		*new_path;
	char		**paths;
	struct stat	s;

	if (!(temp = find_value("PATH", envs)))
		return (NULL);
	paths = ft_split(temp, ':');
	idx = -1;
	while (paths[++idx])
	{
		temp = ft_strjoin("/", argv);
		new_path = ft_strjoin(paths[idx], temp);
		free(temp);
		if (stat(new_path, &s) == 0)
		{
			free_double_arr(paths);
			return (new_path);
		}
		free(new_path);
	}
	free_double_arr(paths);
	return (ft_strdup(argv));
}