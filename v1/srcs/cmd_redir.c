#include "minishell.h"

static int	parse_redir(char *command, t_redir *r)
{
	int		i;
	int		j;
	int		start;
	char	*temp;

	i = -1;
	j = 0;
	start = 0;
	while (command[++i])
	{
		if (ft_strchr("><", command[i]))
		{
			if (j > 0 && ((r->types[0] == BREDIR && command[i] != '<') || 
				(r->types[0] != BREDIR && command[i] == '<')))
					return (0);
			r->argv[j] = substr_and_trim(command, start, i - start, " ");			
			if (j == 0)
				r->cmds = ft_split(r->argv[j], ' ');
			r->types[j] = find_redir_type(command, i);
			r->types[j++] == DREDIR ? i++ : 0;
			start = i + 1;
		}
	}
	r->argv[j] = substr_and_trim(command, start, i - start, " ");
	if (has_syntax_error(r->argv[j]))
		return (-1);
	r->types[j++] = 0;
	r->argv[j] = 0;
	return (1);
}		
// 0, -1 리턴값에서 프리 

// int		get_fd(t_redir *r, int *i)
// {
// 	int fd;

// 	*i = 0;
// 	fd = 0;
// 	while (r->argv[++(*i)])
// 	{
// 		if (*i > 0)
// 			close(fd);
// 		if (r->types[*i] == REDIR)
// 			fd = open(r->argv[*i + 1], O_WRONLY | O_CREAT, 0744);
// 		else if (r->types[*i] == DREDIR)
// 			fd = open(r->argv[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
// 		else
// 			fd = open(r->argv[*i + 1], O_RDWR | O_CREAT, 0644);			
// 	}
// 	return (fd);
// }

void	cmd_redir(t_redir *r, t_list *envs)
{
	int		i;
	int		fd;
	int		status;
	pid_t	child;
	char	*path;

	i = 0;
	child = fork();
	if (child == 0)
	{
		fd = 0;
		while (r->argv[++i])
		{
			if (i > 1)
				close(fd);
			if (r->types[i - 1] == REDIR)
				fd = open(r->argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0744);
			else if (r->types[i - 1] == DREDIR)
				fd = open(r->argv[i], O_WRONLY | O_CREAT | O_APPEND, 0744);
			else
                fd = open(r->argv[i], O_RDONLY, 0644);
		}
        if (fd < 0)
        {
            ft_putstr_fd(r->argv[i - 1], 1);
			ft_putendl_fd(": No such file or directory", 1);
			exit(EXIT_FAILURE);
        }
		if (!(path = find_path(r->cmds[0], envs)))
		{
			ft_putstr_fd(r->cmds[0], 1);
			ft_putendl_fd(": command not found", 1);
			exit(EXIT_FAILURE);
		}
        if (r->types[i - 2] == BREDIR)
            dup2(fd, STDIN_FILENO);
        else
		    dup2(fd, STDOUT_FILENO);
		close(fd);
        if (!exec_builtin(r->cmds, envs))
        {
            if (execve(path, r->cmds, g_envp) == -1)
            {
                ft_putstr_fd(r->cmds[0], 1);
                ft_putendl_fd(": command not found", 1);
                free(path);
                exit(EXIT_SUCCESS);
            }
        }
		free(path);
        exit(EXIT_SUCCESS);
	}
	else
	{
		wait(&status);
		g_exit_value = status / 256;
	}
}

void	exec_redir(t_cmd *cmd, t_list *envs)
{
	int		ret;
	t_redir r;

	init_redir(cmd->command, &r);
	if ((ret = parse_redir(cmd->command, &r)) <= 0)
	{
		if (ret < 0)
			ft_putendl_fd("syntax error near unexpected token `newline'", 1);
		return;
	}
    if (has_quote(r.cmds))
		r.cmds = modify_argv(r.cmds, envs);
	cmd_redir(&r, envs);
}
