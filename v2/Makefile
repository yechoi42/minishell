NAME	=	minishell

SRCS	= 	main.c

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
