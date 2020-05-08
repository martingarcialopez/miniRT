NAME	= libft.a

LIBFT_F	= ft_strlen.c \
		  ft_strcat.c \
		  ft_strncat.c \
		  ft_strlcat.c \
		  ft_strcmp.c \
		  ft_strncmp.c \
		  ft_strcpy.c \
		  ft_strncpy.c \
		  ft_strlcpy.c \
		  ft_atoi.c \
		  ft_strdup.c \
		  ft_strstr.c \
		  ft_strnstr.c \
		  ft_strchr.c \
		  ft_strrchr.c \
		  ft_isalpha.c \
		  ft_isdigit.c \
		  ft_isalnum.c \
		  ft_isascii.c \
		  ft_isprint.c \
		  ft_toupper.c \
		  ft_tolower.c \
		  ft_calloc.c \
		  ft_strrev.c \
		  ft_memset.c \
		  ft_bzero.c \
		  ft_memcpy.c \
		  ft_memccpy.c \
		  ft_memchr.c \
		  ft_memcmp.c \
		  ft_memmove.c \
		  ft_strjoin.c \
		  ft_putchar.c \
		  ft_putchar_fd.c \
		  ft_putstr.c \
		  ft_putstr_fd.c \
		  ft_putendl.c \
		  ft_putendl_fd.c \
		  ft_putnbr.c \
		  ft_putnbr_fd.c \
		  ft_strmapi.c \
		  ft_split.c \
		  ft_itoa.c \
		  ft_strrev.c \
		  ft_substr.c \
		  ft_strtrim.c

PRINTF_F = ft_printf.c \
		   ft_vprintf.c \
		   flag_parsing.c \
		   text_format.c \
		   integer_format.c \
		   float_format.c \
		   conversions.c \
		   utils.c \
		   float_utils.c


SRCS	=  $(addprefix srcs/libft/, $(LIBFT_F)) $(addprefix srcs/ft_printf/, $(PRINTF_F))

OBJS	= ${SRCS:.c=.o}

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -I includes 

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
			ar rc ${NAME} ${OBJS}
			ranlib ${NAME}

all:		${NAME}

clean:
			${RM} ${OBJS}


fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
