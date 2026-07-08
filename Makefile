# **************************************************************************** #
#                                                                              #
#                                                        :::      ::::::::     #
#    Makefile                                          :+:      :+:    :+:     #
#                                                    +:+ +:+         +:+       #
#    By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+          #
#                                                +#+#+#+#+#+   +#+             #
#    Created: 2026/07/08 19:41:38 by sfurst           #+#    #+#               #
#    Updated: 2026/07/08 19:51:43 by sfurst          ###   ########.fr         #
#                                                                              #
# **************************************************************************** #

NAME		= codexion

CC		= cc
CFLAGS		= -Wall -Wextra -Werror
CPPFLAGS	= -MMD -MP
LDFLAGS		=
LDLIBS		=
DEBUG		?= 1

ifeq ($(DEBUG),1)
CFLAGS		+= -g3
CPPFLAGS	+= -DDEBUG=1
endif
RM		= rm -f

# Optional libs: no configured optional library directory detected.
LIBS		=

JOBS ?= $(shell nproc)
MAKEFLAGS += -j $(JOBS) -l $(JOBS)

ifeq ($(DEBUG),1)
CFLAGS		+= -g3
CPPFLAGS	+= -DDEBUG=1
endif

SRC_DIR		= src
SRCS		= $(SRC_DIR)/arg/arg.c \
			  $(SRC_DIR)/main.c \
			  $(SRC_DIR)/sim/coder_init.c \
			  $(SRC_DIR)/sim/dongle_init.c \
			  $(SRC_DIR)/sim/fails.c \
			  $(SRC_DIR)/sim/heap_init.c \
			  $(SRC_DIR)/sim/init_simulation.c \
			  $(SRC_DIR)/sim/sim.c \
			  $(SRC_DIR)/sim/sim_cleanup.c \
			  $(SRC_DIR)/sim/thread_join.c \
			  $(SRC_DIR)/sim/thread_start.c \
			  $(SRC_DIR)/utils/utils.c

OBJS		= $(SRCS:.c=.o)
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) $(LDLIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re
.DEFAULT_GOAL := all
