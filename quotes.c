typedef struct s_quote
{
	int	q_start;
	int	q_end;
	int	dq_start;
	int	dq_end;
	int	start;
	int end;
} t_quote;

void	free_word(void *content)
{
	free(((char *)content));
}

void	free_words(t_list **words)
{
	if (*words)
		ft_lstiter(*words, free_word);
	*words = NULL;
}

void	init_q(t_quote *q)
{
	q->q_start = -1;
	q->q_end = -1;
	q->dq_start = -1;
	q->dq_end = -1;
	q->start = -1;
	q->end = -1;
}

void	check_quotes(char *argv, int i, t_quote *q)
{
	if (argv[i] == '\'')
	{
		if (q->q_start < 0)
			q->q_start = i;
		else
		{
			q->q_end = i;
			q->start = q->q_start;
			q->end = q->q_end;
		}
	}
	else if (argv[i] == '\"')
	{
		if (q->dq_start < 0)
		{
			q->dq_start = i;
		}
		else
		{
			q->dq_end = i;
			q->start = q->dq_start;
			q->end = q->dq_end;
		}
	}
}

char	*make_sentence(t_list *words)
{
	int		num;
	int		len;
	char	*ret;
	char	*temp;

	ret = "";
	while (words)
	{
		temp = ft_strjoin(ret, ((char *)words->content));
		if (*ret)
			free(ret);
		ret = temp;
		words = words->next;
	}
	return (ret);
}

char	*parse_quotes(char *argv)
{
	int		i;
	char	*word;
	char	*ret;
	t_list	*words;
	t_quote	q;

	i = 0;
	init_q(&q);
	while (argv[i])
	{
		check_quotes(argv, i, &q);
		if (q.end > 0)
		{
			word = ft_substr(argv, q.start + 1, q.end - q.start - 1);
			ft_lstadd_back(&words, ft_lstnew(word));
			init_q(&q);
		}
		i++;
	}
	ret = make_sentence(words);
	//free_words(&words);
	return(ret);
}

char	*parse_argv(char *argv, t_list *envs)
{
	char	*temp;
	char	*value;

	if (ft_strchr(argv,'\'') || ft_strchr(argv, '\"'))
	{
		temp = parse_quotes(argv);
		free(argv);
	}
	if (ft_strchr(argv, '$'))
	{
		value = find_value(ft_strchr(argv, '$') + 1, envs);
		return (ft_strdup(value));
	}
	return (ft_strdup(temp));
}

void	deal_argv(char **argv, t_list *envs)
{
	int		i;
	char	*temp;

	i = 0;
	while (argv[++i])
	{
		temp = parse_argv(argv[i], envs);
		//free(argv[i]);
		argv[i] = temp;
	}
}