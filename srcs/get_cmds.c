
#include "minishell.h"

static void		free_cmds(void *content)
{
	free(((t_cmd *)content)->command);
}

static void		add_cmd_to_list(char *line, t_list **cmds, int start, int end)
{
	t_cmd *cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->command = ft_substr(line, start, end - start);
	// cmd->pipe = find_pipe(cmd->command);
	// cmd->redir = find_redir(cmd->command);
	ft_lstadd_back(cmds, ft_lstnew(cmd));
}

static void		input_from_prompt(char **line)
{
	char *temp;

	get_next_line(0, line);
	temp = ft_strtrim(*line, " ");
	free(*line);
	*line = temp;
}

void			init_cmds(t_list **cmds)
{
	if (*cmds)
		ft_lstiter(*cmds, free_cmds);
	*cmds = NULL;
}

t_list			*get_cmds(char *line)
{
	int		idx;
	int		start;
	int		end;
	t_list 	*cmds;

	idx = 0;
	start = 0;
	end = 0;
	cmds = 0;
	input_from_prompt(&line);
	if (!ft_strncmp(line, "\n", 1))
	{
		free(line);
		return (NULL);
	}
	while (line[idx])
	{
		if (line[idx] == ';')
		{
			end = idx;
			add_cmd_to_list(line, &cmds, start, end);
			start = idx + 1;
		}
		idx++;
	}
	if (line[idx - 1] != ';')
		add_cmd_to_list(line, &cmds, start, idx);
	free(line);
	return (cmds);
}
