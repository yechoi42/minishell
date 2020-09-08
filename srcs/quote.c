
#include "minishell.h"

static void		init_quote(t_quote *q)
{
	q->type = INIT;
	q->start = INIT;
	q->end = INIT;
}

static void		check_end(char *argv, int idx, t_quote *q)
{
	if ((argv[idx] == '\'' && q->type == QUOTE) ||
	(argv[idx] == '\"' && q->type == DQUOTE))
		q->end = idx - 1;
	else if ((argv[idx + 1] == '\0' && q->type == QUOTE) ||
	(argv[idx + 1] == '\0' && q->type == DQUOTE))
		q->end = idx;
	else if (q->type == ETC &&
	(argv[idx + 1] == '\'' || argv[idx + 1] == '\"' || argv[idx + 1] == '\0'))
		q->end = idx;
	return;
}

static void		check_quote(char *argv, int idx, t_quote *q)
{
	if (q->type == INIT)
	{
		if (argv[idx] == '\'')
		{
			q->type = QUOTE;
			q->start = idx + 1;
		}
		else if (argv[idx] == '\"')
		{
			q->type = DQUOTE;
			q->start = idx + 1;
		}
		else
		{
			q->type = ETC;
			q->start = idx;
		}
		return ;
	}
	check_end(argv, idx, q);
}

static char		*parse_argv(char *argv, t_list *envs)
{
	int		idx;
	char	*tmp[3];
	t_quote	q;

	idx = 0;
	tmp[0] = "";
	init_quote(&q);
	while (argv[idx])
	{
		check_quote(argv, idx, &q);
		if (q.end != INIT)
		{
			tmp[1] = ft_substr(argv, q.start, q.end - q.start + 1);
			tmp[2] = ft_strjoin(tmp[0], tmp[1]);
			if (tmp[0] && *tmp[0])
				free(tmp[0]);
			if (tmp[1])
				free(tmp[1]);
			tmp[0] = tmp[2];
			init_quote(&q);
		}
		idx++;
	}
	return (tmp[0]);
}

char			**modify_argv(char **argv, t_list *envs)
{
	int		idx;
	int		len;
	char 	**ret;

	len = 0;
	while (argv[len])
		len++;
	ret = (char **)malloc(sizeof(char *) * (len + 1));
	ret[0] = ft_strdup(argv[0]);
	ret[len] = NULL;
	idx = 1;
	while (argv[idx])
	{
		ret[idx] = parse_argv(argv[idx], envs);
		free(argv[idx]);
		idx++;
	}
	return (ret);
}

int				has_quote(char **argv)
{
	int i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '\'' || argv[i][j] == '\"')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}