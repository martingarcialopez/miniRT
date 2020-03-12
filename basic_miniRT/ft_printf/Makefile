NAME	= libftprintf.a

SRCDIR = srcs/

FILES = ft_printf.c ft_vprintf.c flag_parsing.c text_format.c integer_format.c float_format.c conversions.c utils.c float_utils.c

INCLUDES = includes

SRCS	=  $(addprefix $(SRCDIR), $(FILES))

BSRCS	= $(BFILES)

OBJS	= ${SRCS:.c=.o}

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Werror -Wextra -I ${INCLUDES}

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			make -C./libft
			cp libft/libft.a . && ar rc libft.a ${OBJS} && mv libft.a ${NAME}
			ranlib ${NAME}
#-L libft -lft
all:		${NAME}

clean:
			make clean -C libft
			${RM} ${OBJS}

fclean:		
			make fclean -C libft
			${RM} ${OBJS}
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
