# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/09 12:42:27 by msavelie          #+#    #+#              #
#    Updated: 2024/11/15 17:04:29 by msavelie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

NAME = pipex

SRC_DIR = ./srcs
BONUS_DIR = ./bonus

SRCS = \
	${SRC_DIR}/main.c \
	${SRC_DIR}/parse.c \
	${SRC_DIR}/processes.c \
	${SRC_DIR}/clean.c \
	${SRC_DIR}/utils.c \
	${SRC_DIR}/errors.c \
	${SRC_DIR}/validate.c

BONUS = \
	${BONUS_DIR}/main_bonus.c \
	${BONUS_DIR}/processes_bonus.c \
	${BONUS_DIR}/parse_bonus.c \
	${BONUS_DIR}/clean_bonus.c \
	${BONUS_DIR}/utils_bonus.c \
	${BONUS_DIR}/errors_bonus.c \
	${SRC_DIR}/validate.c

OBJS = ${SRCS:.c=.o}
BONUS_OBJS = ${BONUS:.c=.o}

CFLAGS = -g -Wall -Werror -Wextra

LIBFT_NAME = ./libft_updated/libft.a
LIBFT_DIR = ./libft_updated

RM = rm -rf

.PHONY = all clean fclean re bonus

all: ${LIBFT_NAME} ${NAME}

${LIBFT_NAME}:
	@make -C ${LIBFT_DIR} --no-print-directory

${NAME}: ${OBJS}
	@echo "$(MAGENTA)🗂  Copying libft 🗂$(DEF_COLOR)"
	@cp ${LIBFT_DIR}/libft.a .
	@echo "$(BLUE)🛠  Compiling pipex... 🛠$(DEF_COLOR)"
	@cc ${CFLAGS} ${OBJS} libft.a -o ${NAME}
	@echo "$(GREEN)🥳 Success!🥳$(DEF_COLOR)"

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)🚽 Deleting object files... 🚽$(DEF_COLOR)"
	@${RM} ${OBJS}
	@${RM} ${BONUS_OBJS}
	@make clean -C ${LIBFT_DIR} --no-print-directory

fclean: clean
	@make fclean -C ${LIBFT_DIR} --no-print-directory
	@${RM} libft.a
	@echo "$(RED)🪦 Deleting pipex... 🪦$(DEF_COLOR)"
	@${RM} ${NAME}
	@echo "$(RED)☣️  CLEAR ☣️$(DEF_COLOR)"
	@${RM} .bonus

re: fclean all

bonus: all .bonus

.bonus: ${BONUS_OBJS} ${LIBFT_NAME}
	@touch .bonus
	@${RM} ${NAME}
	@echo "$(BLUE)🛠  Compiling pipex with bonus... 🛠$(DEF_COLOR)"
	@cc ${CFLAGS} ${BONUS_OBJS} libft.a -o ${NAME}
	@echo "$(GREEN)🥳 Success!🥳$(DEF_COLOR)"