#include "minishell.h"

static void		delete_key(char *argv, t_list *envs)
{
	t_list *curr;
	t_list *next;

	curr = envs;
	while (curr->next)
	{
		if (is_exist_key(argv, curr->next)) // key�� �����Ѵٸ�?
		{
			next = curr->next;
			curr->next = next->next;
			free(((t_env *)next->content)->key);
			free(((t_env *)next->content)->value);
			free(next->content);
			free(next);
			return ;
		}
		curr = curr->next;
	}
}

void			cmd_unset(char **argv, t_list *envs) // unset ���ɾ� ����
{
	argv++;
	while (*argv)
	{
		delete_key(*argv, envs);
		argv++;
	}
}
