#include "minishell.h"

void	ft_puterror_fd(char *s1, char *s2, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putendl_fd(s2, fd);
}

char	*substr_and_trim(char *command, int start, int num, char *charset)
{
	char	*temp;
	char	*temp2;

	temp = ft_substr(command, start, num);
	temp2 = ft_strtrim(temp, charset);
	free(temp);
	return (temp2);
}

void	free_double_arr(char **arr)
{
	int		idx;

	idx = -1;
	while (arr[++idx])
		free(arr[idx]);
	free(arr);
}