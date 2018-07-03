NAME	=	poll
CC	=	gcc
CFLAGS	=
SRC	=	main.c
OBJ	=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
		@rm -f $(NAME)
		@echo "Deleting $(NAME) binary"

fclean:		clean
		@rm $(OBJ)
		@echo "Deleting $(OBJ)"

re:		fclean all

.PHONY:		all re clean fclean
