# **************************************************************************** #
#                                                                              #
#                                                        :::      ::::::::     #
#    Makefile                                          :+:      :+:    :+:     #
#                                                    +:+ +:+         +:+       #
#    By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+          #
#                                                +#+#+#+#+#+   +#+             #
#    Created: 2026/07/08 19:41:38 by sfurst           #+#    #+#               #
#    Updated: 2026/07/10 19:48:05 by sfurst          ###   ########.fr         #
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
			  $(SRC_DIR)/sim/release.c \
			  $(SRC_DIR)/sim/sim.c \
			  $(SRC_DIR)/sim/sim_cleanup.c \
			  $(SRC_DIR)/sim/sim_helpers.c \
			  $(SRC_DIR)/sim/sim_stuff.c \
			  $(SRC_DIR)/sim/thread_join.c \
			  $(SRC_DIR)/sim/thread_start.c \
			  $(SRC_DIR)/utils/utils.c

OBJ_DIR		= obj

OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) $(LDLIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re compiledb
.DEFAULT_GOAL := all

compiledb:
	make fclean
	compiledb -n make
