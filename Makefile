# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/17 01:29:02 by dbouizem          #+#    #+#              #
#    Updated: 2025/06/02 19:10:55 by dbouizem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a
NAME_BONUS = get_next_line_bonus.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE)

SRCS = \
		get_next_line.c \
		get_next_line_utils.c \

OBJS = $(SRCS:.c=.o)

SRCS_BONUS = \
		get_next_line_bonus.c \
		get_next_line_utils_bonus.c \

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

ifndef BUFFER_SIZE
	BUFFER_SIZE = 42
endif

all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(OBJS)
	ar rcs $@ $^

$(NAME_BONUS): $(OBJS_BONUS)
	ar rcs $@ $^

%.o: %.c get_next_line.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c get_next_line_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -f $(OBJS) $(OBJS_BONUS)

fclean : clean
	@rm -rf $(NAME) $(NAME_BONUS)

re : fclean all

.PHONY : all clean fclean re bonus

