
#include "minishell.h"

void		show_art(void)
{
	int		fd;
	char	*line;

	fd = open("art_header.txt", O_RDONLY);
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


static void		input_from_prompt(char **line)
{
	char *temp;

	get_next_line(0, line);
	if (*line != NULL)
	{
		temp = ft_strtrim(*line, " ");
		free(*line);
		*line = temp;
	}
}

void		show_prompt(t_list *envs, char **line)
{
	char	*user;
	char	*pwd;
	char	*tmp;
	char	*set;

	user = find_value("USER", envs);
	tmp = getcwd(0, 1024);
	set = ft_strjoin("/Users/", user);
	pwd = ft_strtrim(tmp, set);
	free(tmp);
	ft_putstr_fd(user, 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd("\033[44m", 1); // �ϴû� ���
	ft_putstr_fd("\033[30m", 1); // ������ ��Ʈ
	ft_putstr_fd(" ~/", 1);
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd(" ", 1);
	ft_putstr_fd("\033[0m", 1); // ���� �� ����
	ft_putstr_fd(" $ ", 1);
	free(pwd);
	input_from_prompt(line);
}
