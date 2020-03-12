NAME	= miniRT

HEAD	= includes/

OBJS	= \
		srcs/minirt.o \
		srcs/parsing.o \
		srcs/vector_operations.o \
		srcs/vector_operations2.o\
		srcs/concerning_light.o \
		srcs/intersections.o \
		srcs/utils.o

#OBJS	= ${SRCS:.c=.o}

CC		= gcc -I $(HEAD)

RM		= rm -f

FLAGS	= -framework OpenGL -framework AppKit #-I includes/

#.c.o:
#		${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
	${CC} -g ${FLAGS} $(OBJS) $(HEAD)libftprintf.a $(HEAD)libmlx.a -o ${NAME}

all:		${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
