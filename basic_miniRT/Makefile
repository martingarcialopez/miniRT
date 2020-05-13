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


#######${CC} ${CFLAGS} ${FLAGS} $(OBJS) -L $(LIB)libft -lft -L $(LIB)libvector -lvct -L $(LIB)minilibx_opengl_20191021 -lmlx -o ${NAME}




FLAGS = -L $(LIB)libft -lft -L $(LIB)libvector -lvct

MACOS_FLAGS	= -L $(LIB)minilibx_opengl_20191021 -lmlx -framework OpenGL -framework AppKit 

LINUX_FLAGS = -L $(LIB)minilibx-linux -lmlx -lm -lX11 -lXext 

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	FLAGS += $(MACOS_FLAGS)
endif
ifeq ($(UNAME),Linux)
	FLAGS += $(LINUX_FLAGS)
endif


${NAME}:	${OBJS}
			make -C $(LIB)libft
			make -C $(LIB)libvector
###	${CC} ${CFLAGS}  $(OBJS) -lm -lX11 -lXext -L $(LIB)libft -lft -L $(LIB)libvector -lvct -L $(LIB)minilibx-linux -lmlx -o ${NAME}
			${CC} ${CFLAGS}  $(OBJS) $(FLAGS) -o ${NAME}

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
