#include "minishell.h"

static int			print_exit_status()
{
	ft_putstr_fd(ft_itoa(g_exit_value), 1);
	ft_putendl_fd(": command not found", 1);
	return (1);
}

int		exec_builtin(char *line, t_list *envs)
{
	char **argv;

	if (line[0] == '$' && line[1] == '?' && print_exit_status())
		return (1);
	argv = get_argv(line, envs);
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
		cmd_pwd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		cmd_env(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
		cmd_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
		cmd_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen(argv[0])))
		cmd_exit(argv, envs);
	else
	{
		free_double_arr(argv);
		return (0);
	}
	free_double_arr(argv);
	return (1);
}

void			exec_others(char *line, t_list *envs)
{
	int		status;
	char	*path;
	char	**argv;
	pid_t	child;

	argv = get_argv(line, envs);
	path = find_path(argv[0], envs);
	if (!path)
	{
		ft_puterror_fd(argv[0], ": command not found", 2);
		return;
	}
	child = fork();
	if (child == 0)
	{
		if (execve(path, argv, g_envp) == -1)
		{
			ft_puterror_fd(argv[0], ": command not found", 2);
			exit(127);
		}
		exit(EXIT_SUCCESS);
	}
	wait(&status);
	free(path);
	free_double_arr(argv);
	g_exit_value = status / 256;
}

void	exec_cmds(char *line, t_list *envs)
{
	if (has_pipe(line))
	 	exec_pipe(line, envs);
	else if (has_redir(line))
		exec_redir(line, envs);
	else if (!exec_builtin(line, envs))
		exec_others(line, envs);
}