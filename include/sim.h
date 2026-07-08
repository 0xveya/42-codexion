/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.h                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:48:40 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:30:51 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIM_H
# define SIM_H

# include "args.h"
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>

typedef enum e_state
{
	taking_dongle,
	compiling,
	debugging,
	refactoring,
	burned_out
}							t_state;

typedef struct s_dongle
{
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	bool					available;
	int64_t					released_at;
}							t_dongle;

typedef struct s_coder
{
	uint32_t				id;
	pthread_t				thread;
	t_state					state;
	int64_t					last_compile_start;
	uint32_t				compiles_done;
	t_dongle				*left;
	t_dongle				*right;
	struct s_app			*app;
}							t_coder;

typedef struct s_heap
{
	t_coder					**data;
	uint32_t				size;
	uint32_t				capacity;
}							t_heap;

typedef struct s_app
{
	t_args					args;
	t_coder					*coders;
	t_dongle				*dongles;
	t_heap					sched_heap;
	pthread_mutex_t			log_mutex;
	pthread_mutex_t			state_mutex;
	bool					simulation_stop;
	int64_t					start_time;
	pthread_t				monitor_thread;
}							t_app;

typedef enum e_init_status
{
	init_ok,
	init_err
}							t_init_status;

typedef union u_init_data
{
	t_app					*success;
	const char				*error_msg;
}							t_init_data;

typedef struct s_init_result
{
	t_init_status			status;
	t_init_data				data;
}							t_init_result;

typedef enum e_dongle_init_status
{
	dongle_init_ok,
	dongle_init_err
}							t_dongle_init_status;

typedef union u_dongle_init_data
{
	t_dongle				*success;
	const char				*error_msg;
}							t_dongle_init_data;

typedef struct s_dongle_init_result
{
	t_dongle_init_status	status;
	t_dongle_init_data		data;
}							t_dongle_init_result;

typedef enum e_coder_init_status
{
	coder_init_ok,
	coder_init_err
}							t_coder_init_status;

typedef union u_coder_init_data
{
	t_coder					*success;
	const char				*error_msg;
}							t_coder_init_data;

typedef struct s_coder_init_result
{
	t_coder_init_status		status;
	t_coder_init_data		data;
}							t_coder_init_result;

typedef enum e_heap_init_status
{
	heap_init_ok,
	heap_init_err
}							t_heap_init_status;

typedef union u_heap_init_data
{
	t_heap					success;
	const char				*error_msg;
}							t_heap_init_data;

typedef struct s_heap_init_result
{
	t_heap_init_status		status;
	t_heap_init_data		data;
}							t_heap_init_result;

void						free_coders(t_coder *coders, uint32_t count);
void						free_dongles(t_dongle *dongles, uint32_t count);
void						free_heap(t_heap *heap);
t_init_result				init_simulation(const t_args *args);

typedef enum e_start_status
{
	sim_start_ok,
	sim_start_err
}							t_start_status;

typedef union u_start_data
{
	int						dummy;
	const char				*error_msg;
}							t_start_data;

typedef struct s_start_result
{
	t_start_status			status;
	t_start_data			data;
}							t_start_result;

void						*coder_routine(void *arg);
void						join_simulation(t_app *app);
void						*monitor_routine(void *arg);
t_start_result				start_simulation(t_app *app);

#endif
