NAME	=	minishell

SRCS	= 	srcs/show_prompt_art.c \
			srcs/get_cmds.c \
			srcs/exec_cmds.c \
			srcs/quote.c \
			srcs/cmd_cd.c \
			srcs/cmd_env.c \
			srcs/cmd_export.c \
			srcs/cmd_export_utils.c \
			srcs/cmd_unset.c \
			srcs/cmd_exit.c \
			srcs/cmd_others.c \
			srcs/cmd_redir.c \
			srcs/cmd_redir_utils.c \
			srcs/utils.c \
			srcs/main.c

LEAKS	=	-g3 -fsanitize=address

LIBS	=	-L./libs/libft -lft

HEADER	=	-I./includes

CC		=	gcc

RM		=	rm -rf

all		: $(NAME)

$(NAME)	: $(LIBFT)
	$(CC) $(SRCS) $(LIBS) $(HEADER) $(LEAKS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C ./libs/libft

clean	:
	$(MAKE) -C ./libs/libft clean
	rm -rf $(OBJS)

fclean	:
	$(MAKE) -C ./libs/libft clean
	rm -rf $(NAME)

re		: fclean all
