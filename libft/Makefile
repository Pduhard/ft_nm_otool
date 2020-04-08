# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aplat <aplat@student.le-101.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/27 02:23:54 by pduhard-          #+#    #+#              #
#    Updated: 2020/03/04 07:10:23 by aplat            ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#__________CONFIG__________#

CC			= gcc
NAME		= libft.a

FLAGS		= -Wall -Werror -Wextra -O3
INCLUDES	= -I ./libft.h

SRCDIR		= ./
BINDIR		= ./

SRC			=	ft_putchar.c		\
				ft_putstr.c			\
				ft_tolower.c		\
				ft_isalnum.c		\
				ft_isalpha.c		\
				ft_isascii.c		\
				ft_isdigit.c		\
				ft_isprint.c		\
				ft_isspace.c		\
				ft_toupper.c		\
				ft_atoi.c			\
				ft_atoll.c			\
				ft_atold.c			\
				ft_strcpy.c			\
				ft_strncpy.c		\
				ft_strcat.c			\
				ft_strncat.c		\
				ft_strlcat.c		\
				ft_strcmp.c			\
				ft_strncmp.c		\
				ft_strstr.c			\
				ft_strnstr.c		\
				ft_strchr.c			\
				ft_strichr.c		\
				ft_strrchr.c		\
				ft_strlen.c			\
				ft_strdup.c			\
				ft_strndup.c		\
				ft_memset.c			\
				ft_bzero.c			\
				ft_memcpy.c			\
				ft_memccpy.c		\
				ft_memmove.c		\
				ft_memchr.c			\
				ft_memcmp.c			\
				ft_memalloc.c		\
				ft_memdel.c			\
				ft_strnew.c			\
				ft_strncmp_case.c	\
				ft_strfillnew.c		\
				ft_strdel.c			\
				ft_strclr.c			\
				ft_striter.c		\
				ft_striteri.c		\
				ft_strmap.c			\
				ft_strmapi.c		\
				ft_strequ.c			\
				ft_strnequ.c		\
				ft_strsub.c 		\
				ft_strfsub.c 		\
				ft_strjoin.c		\
				ft_strfjoin.c		\
				ft_strtrim.c		\
				ft_strsplit.c		\
				ft_itoa.c			\
				ft_lltoa.c			\
				ft_ldtoa.c			\
				ft_ldtoa1.c			\
				ft_ulltoa.c			\
				ft_putendl.c		\
				ft_putnbr.c			\
				ft_putchar_fd.c		\
				ft_putstr_fd.c		\
				ft_putendl_fd.c		\
				ft_putnbr_fd.c		\
				ft_lstnew.c			\
				ft_lstdelone.c		\
				ft_lstdel.c			\
				ft_lstadd.c			\
				ft_lstiter.c		\
				ft_lstmap.c			\
				ft_countparts.c		\
				ft_pow.c			\
				ft_sqrt.c			\
				ft_abs.c			\
				ft_fabs.c			\
				ft_isupper.c		\
				ft_islower.c		\
				ft_memrcpy.c		\
				ft_emptystr.c		\
				ft_baseconvert.c	\
				get_next_line.c		\
				ft_ifree.c			\
				ft_printf.c			\
				ft_getf.c			\
				ft_fdprintf.c		\
				ft_fdvprintf.c		\
				parse_conv.c		\
				parsing.c			\
				signed_conv.c		\
				signed_conv1.c		\
				unsigned_conv.c		\
				unsigned_conv1.c	\
				octal_conv.c		\
				octal_conv1.c		\
				hex_conv.c			\
				hex_conv1.c			\
				binary_conv.c		\
				float_conv.c		\
				char_conv.c			\
				char_conv1.c		\
				percent_conv.c		\
				float_conv.c		\
				scientist_conv.c	\
				g_fore_conv.c		\
				n_conv.c			\
				numeric_flags.c		\
				signed_flags.c		\
				str_flags.c			\
				font.c				\
				ft_swap.c			\


BIN			= $(SRC:.c=.o)

SRCS		= $(addprefix $(SRCDIR), $(SRC))
BINS		= $(addprefix $(BINDIR), $(BIN))

COMPILE		= $(CC) $(FLAGS) $(INCLUDES)

.PHONY: all clean fclean re

#__________COLORS__________#

R			=	\033[0;31m
G			=	\033[32;7m
B			=	\033[0;34m
N			=	\33[0m

#__________RULES__________#

all: $(NAME)

$(NAME): $(BINS)

	@ar rc $(NAME) $^
	@echo "\n\n${B}[LIBRARY \"$(NAME)\" COMPILED]\n"
	@ranlib $(NAME)
	@echo "${B}[LIBRARY \"$(NAME)\" INDEXED]\n"

$(BINDIR)%.o: $(SRCDIR)%.c libft.h ft_printf.h

	@$(COMPILE) -o $@ -c $< && echo "${G} \c"

clean:

	@echo "\n${R}[CLEANING $(NAME) BINARIES]\n"
	@rm -f $(BINS)

fclean: clean

	@echo "${R}[REMOVING \"$(NAME)\"]\n"
	@rm -f $(NAME)

re: fclean all
