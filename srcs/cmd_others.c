
#include "minishell.h"

static char		*find_path(char *argv, t_list *envs)
{
	int			idx;
	char		*temp;
	char		*new_path;
	char		**paths;
	struct stat	s;

	temp = find_value("PATH", envs);
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
	return (NULL);
}

void			cmd_others(char **argv, t_list *envs)
{
	int		status;
	char	*path;
	pid_t	child;

	path = find_path(argv[0], envs);
	if (!path)
	{
		ft_putstr_fd(argv[0], 1);
		ft_putendl_fd(": command not found", 1);
		return ;
	}
	child = fork();
	if (child == 0)
		execve(path, argv, g_envp);
	else
	{
		wait(0);
		free(path);
	}
}