NAME	= miniRT

HEAD	= includes

SRCDIR	= srcs/

LIB		= lib/

FILES	= minirt.c parsing.c concerning_light.c intersections.c utils.c

SRCS	= $(addprefix $(SRCDIR), $(FILES))

OBJS	= ${SRCS:.c=.o}

CC		= gcc -g

RM		= rm -f

CFLAGS	= -Wall -Wextra -I $(HEAD)

FLAGS	= -framework OpenGL -framework AppKit

${NAME}:	${OBJS}
			make -C $(LIB)libft
			make -C $(LIB)libvector
			${CC} ${CFLAGS} ${FLAGS} $(OBJS) -L $(LIB)libft -lft -L $(LIB)libvector -lvct -L $(LIB)minilibx_opengl_20191021 -lmlx -o ${NAME}

all:		${NAME}

clean:
			make clean -C $(LIB)libft
			make clean -C $(LIB)libvector
			${RM} ${OBJS}

fclean:		clean
			make fclean -C $(LIB)libft
			make fclean -C $(LIB)libvector
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
