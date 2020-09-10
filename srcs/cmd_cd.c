
#include "minishell.h"

void	cmd_cd(char **argv, t_list *envs)
{
	char *path;

	path = 0;
	if (ft_strlen(argv[1]) == 1 && *argv[1] == '~')
		argv[1] = ft_strjoin("$", "HOME");
	if (*argv[1] == '$')
	{
		path = find_value(argv[1] + 1, envs);
		if (chdir(path) == -1)
			ft_putendl_fd(strerror(errno), 2);
		free(path);
		return ;
	}
	else if (argv[1] == NULL)
	{
		path = find_value("HOME", envs);
		if (chdir(path) == -1)
			ft_putendl_fd(strerror(errno), 2);
		free(path);
		return ;
	}
	if (chdir(argv[1]) == -1)
		ft_putendl_fd(strerror(errno), 2);
}