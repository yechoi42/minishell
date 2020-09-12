
#include "minishell.h"

void	cmd_pipe(t_pipe *p, int i, t_list *envs)
{
	int		status;
	int		fd[2];
	pid_t	child;
	char	*path;

	child = fork();
	pipe(fd);
	if (child == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		if (!exec_builtin(p->argv[i], envs))
		{
			if (!(path = find_path(p->argv[i][0], envs)))
			{
				printf("111\n");
				ft_putstr_fd(p->argv[i][0], 1);
				ft_putendl_fd(": command not found", 1);
				exit(EXIT_SUCCESS);
			}
			if (execve(path, p->argv[i], g_envp) == -1)
			{
				printf("222\n");
				ft_putstr_fd(p->argv[i][0], 1);
                ft_putendl_fd(": command not found", 1);
				exit(EXIT_SUCCESS);				
			}
		}
		exit(EXIT_SUCCESS);	
	}
	else
	{
		wait(&status);
		dup2(fd[0], 0);
		close(fd[1]);
			fd[0] = 0;
		g_exit_value = status / 256;
		child = fork();
		if (!exec_builtin(p->argv[++i], envs))
		{
			if (!(path = find_path(p->argv[i][0], envs)))
			{
				ft_putstr_fd(p->argv[i][0], 1);
				ft_putendl_fd(": command not found", 1);
			}
			if (execve(path, p->argv[i], g_envp) == -1)
			{
				ft_putstr_fd(p->argv[i][0], 1);
				ft_putendl_fd(": command not found", 1);			
			}
		}
	}	
}

void     parse_p_argv(t_pipe *p, t_list *envs)
{
    int i;

    i = -1;
    while (p->cmds[++i])
    {
        p->argv[i] = ft_split(p->cmds[i], ' ');
        if (has_quote(p->argv[i]))
            p->argv[i] = modify_argv(p->argv[i], envs);
    }
}

int     parse_p_cmds(char *command, t_pipe *p, t_list *envs)
{
	int		i;
	int		j;
	int		start;
	char 	*temp;

	i = -1;
	j = 0;
	start = 0;
	while (command[++i])
	{
		if (ft_strchr("|", command[i]))
		{
			p->cmds[j] = substr_and_trim(command, start, i - start, " ");
			start = i + 1;
			j++;
		}
	}
	p->cmds[j] = substr_and_trim(command, start, i - start, " ");
	if (!*(p->cmds[j]))
		return (0);
	p->cmds[++j] = NULL; 
	return (1);   
}

void    init_pipe(char *command, t_pipe *p)
{
    int num;

    num = 0;
    while (*command)
    {
        if (ft_strchr("|", *command))
            num++;
        command++;
    }
    p->argc = num + 1;
    p->cmds = (char **)malloc(sizeof(char *) * (p->argc + 1));
    p->argv = (char ***)malloc(sizeof(char **) * (p->argc + 1));
}

void	exec_pipe(t_cmd *cmd, t_list *envs)
{
	int		i;
	int		ret;
	t_pipe	p;

	init_pipe(cmd->command, &p);
	if ((ret = parse_p_cmds(cmd->command, &p, envs)) <= 0)
		return ;
    parse_p_argv(&p, envs);
	i = 0;
	cmd_pipe(&p, i, envs);
}


int		exec_builtin(char **argv, t_list *envs)
{
	if (has_quote(argv))
		argv = modify_argv(argv, envs);
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		cmd_env(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
		cmd_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
		cmd_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen(argv[0])))
		cmd_exit(argv, envs);
	else if (!ft_strncmp(argv[0], "$?", ft_strlen(argv[0])))
		print_exit_status();
	else
		return (0);
	return (1);
}

void			exec_cmds(t_list *cmds, t_list *envs)
{
	char	**argv;

	while (cmds != NULL)
	{
		if (!(argv = ft_split(((t_cmd *)cmds->content)->command, ' ')))
			exit(1);
		if (((t_cmd *)cmds->content)->pipe)
		 	exec_pipe(((t_cmd *)cmds->content), envs);
		else if (((t_cmd *)cmds->content)->redir)
			exec_redir(((t_cmd *)cmds->content), envs);
		else if (!exec_builtin(argv, envs))
			exec_others(argv, envs);
		free_double_arr(argv);
		cmds = cmds->next;
	}
}