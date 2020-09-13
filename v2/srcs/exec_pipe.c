#include "minishell.h"

void	parse_pipe(char **line, t_pipe *p, t_list *envs)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while ((*line)[++i])
	{
		if ((*line)[i] == '|')
		{
			temp = ft_substr(*line, 0, i);
			p->line = ft_strtrim(temp, " ");
			free(temp);
			temp = ft_substr(*line, i + 1, ft_strlen(*line) - i);
			temp2 = ft_strtrim(temp, " ");
			free(temp);
			free(*line);
			*line = temp2;
			p->argv = get_argv(p->line, envs);
			return ;
		}
	}
}

void	exec_pipe(char *line, t_list *envs)
{
	int		i;
	int		fd[2];
	int		child[2];
	int		status[2];
	t_pipe	p;

	parse_pipe(&line, &p, envs);
	pipe(fd);
	child[0] = fork();
	if (child[0] == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_cmds(p.line, envs);
		exit(EXIT_SUCCESS);
	}
	child[1] = fork();
	if (child[1] == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_cmds(line, envs);
		exit(EXIT_SUCCESS);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(child[1], &status[1], 0);
	waitpid(child[0], &status[0], WNOHANG);
}
