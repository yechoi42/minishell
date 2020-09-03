NAME	= minishell

SRCS	= main.c

LEAKS	= -g3 -fsanitize=address

LIBS	= -L./libft -lft 

HEADER	= -I./libft

CC		= gcc

RM		= rm -rf

all		: $(NAME)

$(NAME)	: $(LIBFT)
	$(CC) $(SRCS) $(LIBS) $(HEADER) $(LEAKS)

$(LIBFT): 
	$(MAKE) -C ./libft

clean	:
	$(MAKE) -C ./libft clean
	rm -rf $(OBJS)

fclean	:
	$(MAKE) -C ./libft clean
	rm -rf $(NAME)

re		: fclean all

