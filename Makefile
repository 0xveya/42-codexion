# **************************************************************************** #
#                                                                              #
#                                                        :::      ::::::::     #
#    Makefile                                          :+:      :+:    :+:     #
#                                                    +:+ +:+         +:+       #
#    By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+          #
#                                                +#+#+#+#+#+   +#+             #
#    Created: 2026/07/08 19:41:38 by sfurst           #+#    #+#               #
#    Updated: 2026/07/12 02:22:20 by sfurst          ###   ########.fr         #
#                                                                              #
# **************************************************************************** #

NAME		= codexion

CC			= cc
RM			= rm -rf

CFLAGS		= -Wall -Wextra -Werror -pthread
CPPFLAGS	= -MMD -MP
LDFLAGS		= -pthread
LDLIBS		=

DEBUG		?= 1

ifeq ($(DEBUG),1)
CFLAGS		+= -g3
CPPFLAGS	+= -DDEBUG=1
endif

JOBS ?= $(shell nproc)
MAKEFLAGS += -j $(JOBS) -l $(JOBS)

SRC_DIR		= src
OBJ_DIR		= obj

SRCS		= $(SRC_DIR)/arg/arg.c \
			  $(SRC_DIR)/main.c \
			  $(SRC_DIR)/sim/acquire.c \
			  $(SRC_DIR)/sim/coder_init.c \
			  $(SRC_DIR)/sim/dongle_init.c \
			  $(SRC_DIR)/sim/fails.c \
			  $(SRC_DIR)/sim/heap_helpers.c \
			  $(SRC_DIR)/sim/heap_init.c \
			  $(SRC_DIR)/sim/heap_pop.c \
			  $(SRC_DIR)/sim/heap_push.c \
			  $(SRC_DIR)/sim/heap_remove.c \
			  $(SRC_DIR)/sim/init_simulation.c \
			  $(SRC_DIR)/sim/monitor.c \
			  $(SRC_DIR)/sim/release.c \
			  $(SRC_DIR)/sim/scheduler_compare.c \
			  $(SRC_DIR)/sim/scheduler_pair.c \
			  $(SRC_DIR)/sim/scheduler_ready.c \
			  $(SRC_DIR)/sim/sim.c \
			  $(SRC_DIR)/sim/sim_cleanup.c \
			  $(SRC_DIR)/sim/sim_helpers.c \
			  $(SRC_DIR)/sim/sim_stuff.c \
			  $(SRC_DIR)/sim/sleep.c \
			  $(SRC_DIR)/sim/start_helpers.c \
			  $(SRC_DIR)/sim/thread_join.c \
			  $(SRC_DIR)/sim/thread_start.c \
			  $(SRC_DIR)/utils/printing.c \
			  $(SRC_DIR)/utils/utils.c

OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME) codexion_tsan

re:
	$(MAKE) fclean
	$(MAKE) all

tsan: fclean
	$(MAKE) NAME=codexion_tsan \
		CFLAGS="-Wall -Wextra -Werror -pthread -g3 -fsanitize=thread" \
		LDFLAGS="-pthread -fsanitize=thread" \
		DEBUG=0

compiledb:
	$(MAKE) fclean
	compiledb -n make

-include $(DEPS)

.PHONY: all clean fclean re tsan compiledb
