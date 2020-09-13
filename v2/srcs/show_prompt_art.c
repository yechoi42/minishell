#include "minishell.h"

void		show_art(void)
{
	int		fd;
	char	*line;

	fd = open("header", O_RDONLY);
	while (get_next_line(fd, &line))
	{
		ft_putstr_fd("\033[36m", 1);
		ft_putendl_fd(line, STDOUT_FILENO);
		free(line);
	}
	close(fd);
	free(line);
	ft_putendl_fd("\033[0m", 1);
}

int			input_from_prompt(char **line)
{
	char	*temp;

	if ((get_next_line(0, line) == 0))
	{
		ft_putstr_fd("exit", 1);
		exit(EXIT_SUCCESS);
	}
	if (**line == '\n')
	{
		free(*line);
		return (0);
	}
	if (*line != NULL)
	{
		temp = ft_strtrim(*line, " ");
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
		*line = temp;
	}
	return (1);
}

void		show_prompt(char *user)
{
	char	*pwd;
	char	*tmp;
	char	*tmp2;
	char	*set;

	tmp = getcwd(0, 1024);
	set = ft_strjoin("/Users/", user);
	tmp2 = ft_strtrim(tmp, set);
	pwd = ft_strjoin("~/", tmp2);
	free(tmp2);
	ft_putstr_fd(user, 1);
	ft_putstr_fd(" \033[44m\033[30m ", 1);
	if (ft_strncmp(tmp, set, ft_strlen(tmp)) != 0)
		ft_putstr_fd(pwd, 1);
	else if (ft_strncmp(tmp, "/", ft_strlen(tmp)) == 0)
		ft_putstr_fd("/", 1);
	else if (ft_strncmp(tmp, "/Users", ft_strlen(tmp)) == 0)
		ft_putstr_fd("/Users", 1);
	else if (ft_strncmp(tmp, set, ft_strlen(tmp)) == 0)
		ft_putstr_fd("~", 1);
	ft_putstr_fd(" \033[0m $ ", 1);
	free(set);
	free(tmp);
	free(pwd);
}