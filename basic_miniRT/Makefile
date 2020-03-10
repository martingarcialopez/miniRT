NAME	= miniRT

OBJS	= \
		minirt.o \
		parsing.o \
		vector_operations.o \
		concerning_light.o \
		intersections.o

#OBJS	= ${SRCS:.c=.o}

CC		= gcc

RM		= rm -f

FLAGS	= -framework OpenGL -framework AppKit #-I includes/

#.c.o:
#		${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
	${CC} ${FLAGS} $(OBJS) libft.a libmlx.a -o ${NAME}

all:		${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
