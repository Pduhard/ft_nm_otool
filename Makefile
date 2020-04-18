# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/12/10 21:06:37 by pduhard-     #+#   ##    ##    #+#        #
#    Updated: 2019/03/04 11:32:24 by pduhard-    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

##__________CONFIG__________#

NM_NAME		=	ft_nm
OTOOL_NAME		=	ft_otool

CC			=	gcc
FLAGS		=	-g -Wall -Werror -Wextra -O3 -ffast-math -march=native -flto
LIB_FLAGS	=	-L $(LIB_PATH) $(LIB_FLAG)
NM_INC_DIR	=	nm/includes/
NM_INCLUDES	=	ft_nm.h
OTOOL_INC_DIR	=	otool/includes/
OTOOL_INCLUDES	=	ft_otool.h
COMMON_INC_DIR	=	common/includes/
COMMON_INCLUDES	=	common.h

NM_SRC_PATH	=	./nm/srcs/
OTOOL_SRC_PATH	=	./otool/srcs/
COMMON_SRC_PATH	=	./common/srcs/

NM_BIN_PATH	=	./nm/bins/
OTOOL_BIN_PATH	=	./otool/bins/
COMMON_BIN_PATH	=	./common/bins/
LIB_PATH	=	./libft/

NM_SRC		=			main.c									\
								symbol.c								\
								sort.c									\

COMMON_SRC = 		handle.c								\
								load_command.c					\
								handle_lc_segment.c			\
								handle_lc_segment_64.c	\
								handle_lc_symtab.c			\
								conversions.c						\
								utils.c									\
								mach-o.c								\
								fat.c										\
								archive.c								\
								error.c									\

OTOOL_SRC		=	main.c					\

#COMMON_SRC =	handle.c								\
							handle_lc_segment.c			\
							handle_lc_segment_64.c	\
							handle_lc_symtab.c			\
							mach-o.c								\
							fat.c										\
							archive.c								\

NM_BIN			=	$(NM_SRC:.c=.o)#	$(COMMON_SRC:.c=.o)
OTOOL_BIN		=	$(OTOOL_SRC:.c=.o)# $(COMMON_SRC:.c=.o)
COMMON_BIN	= $(COMMON_SRC:.c=.o)

LIB_FLAG			=	-lft
LIB						=	libft.a

# SRCS1		=	$(addprefix $(NM_SRC_PATH), $(NM_SRC))
# SRCS2		=	$(addprefix $(OTOOL_SRC_PATH), $(OTOOL_SRC))
NM_BINS				=	$(addprefix $(NM_BIN_PATH), $(NM_BIN)) $(addprefix $(COMMON_BIN_PATH), $(COMMON_BIN))
OTOOL_BINS		=	$(addprefix $(OTOOL_BIN_PATH), $(OTOOL_BIN)) $(addprefix $(COMMON_BIN_PATH), $(COMMON_BIN))
# COMMON_BINS		= $(addprefix $(COMMON_BIN_PATH), $(COMMON_BIN))
LIBS					=	$(addprefix $(LIB_PATH), $(LIB))
NM_INCS				= $(addprefix $(NM_INC_DIR), $(NM_INCLUDES))
OTOOL_INCS		= $(addprefix $(OTOOL_INC_DIR), $(OTOOL_INCLUDES))
COMMON_INCS 	= $(addprefix $(COMMON_INC_DIR), $(COMMON_INCLUDES))

.PHONY: all clean fclean re lib libft

#__________COLORS__________#

R			=	\033[0;31m
G			=	\033[32;7m
B			=	\033[0;34m
N			=	\33[0m

#__________RULES__________#

all: make_libft $(NM_NAME) $(OTOOL_NAME)

libft: $(LIBS)

$(NM_NAME): $(LIBS) $(NM_BINS) $(COMMON_BINS)

	@$(CC) -I $(NM_INC_DIR) -I $(COMMON_INC_DIR) $(FLAGS) $(LIB_FLAGS) $^ -o $@
	@echo "\n\n$(B)[EXECUTABLE \"$(NM_NAME)\" READY]\n"

$(OTOOL_NAME): $(LIBS) $(OTOOL_BINS) $(COMMON_BINS)

	@$(CC) -I $(OTOOL_INC_DIR) -I $(COMMON_INC_DIR) $(FLAGS) $(LIB_FLAGS) $^ -o $@
	@echo "\n\n$(B)[EXECUTABLE \"$(OTOOL_NAME)\" READY]\n"

make_libft:

		@make -C $(LIB_PATH)

$(LIBS):

	@make -C $(LIB_PATH)

$(NM_BIN_PATH)%.o: $(NM_SRC_PATH)%.c $(NM_INCS)

	@mkdir -p $(NM_BIN_PATH) || true
	@$(CC) $(FLAGS) -I $(NM_INC_DIR) -o $@ -c $< && echo "${G} \c"

$(OTOOL_BIN_PATH)%.o: $(OTOOL_SRC_PATH)%.c $(OTOOL_INCS)

	@mkdir -p $(OTOOL_BIN_PATH) || true
	@$(CC) $(FLAGS) -I $(OTOOL_INC_DIR) -o $@ -c $< && echo "${G} \c"

$(COMMON_BIN_PATH)%.o: $(COMMON_SRC_PATH)%.c $(COMMON_INCS)

	@mkdir -p $(COMMON_BIN_PATH) || true
	@$(CC) $(FLAGS) -I $(COMMON_INC_DIR) -o $@ -c $< && echo "${G} \c"

clean:

	@make clean -C $(LIB_PATH)
	@echo "${R}[CLEANING $(NM_NAME) BINARIES]\n"
	@rm -Rf $(NM_BIN_PATH)
	@echo "${R}[CLEANING $(OTOOL_NAME) BINARIES]\n"
	@rm -Rf $(OTOOL_BIN_PATH)
	@echo "${R}[CLEANING $(COMMON_NAME) BINARIES]\n"
	@rm -Rf $(COMMON_BIN_PATH)

fclean: clean

	@echo "${R}[REMOVING $(LIB)]"
	@rm -rf $(LIBS)
	@echo "\n${R}[REMOVING \"$(NM_NAME)\"]"
	@rm -f $(NM_NAME)
	@echo "\n${R}[REMOVING \"$(OTOOL_NAME)\"]\n"
	@rm -f $(OTOOL_NAME)

re: fclean all
