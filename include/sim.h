/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.h                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:48:40 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:44:40 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIM_H
# define SIM_H

# include "args.h"
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <time.h>

typedef struct s_coder		t_coder;

typedef struct s_request
{
	t_coder					*coder;
	uint64_t				sequence;
	int64_t					deadline;
}							t_request;

typedef struct s_heap
{
	t_request				*data;
	uint32_t				size;
	uint32_t				capacity;
}							t_heap;

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
	uint64_t				next_sequence;

	t_heap					queue;
}							t_dongle;

typedef struct s_coder
{
	uint32_t				id;
	pthread_t				thread;
	t_state					state;
	int64_t					last_compile_start;
	uint32_t				compiles_done;
	t_request				request;
	bool					waiting;
	bool					granted;
	t_dongle				*left;
	t_dongle				*right;
	struct s_app			*app;
}							t_coder;

typedef struct s_app
{
	t_args					args;
	t_coder					*coders;
	t_dongle				*dongles;

	pthread_mutex_t			log_mutex;
	pthread_mutex_t			state_mutex;
	pthread_cond_t			stop_cond;

	bool					simulation_stop;
	int64_t					start_time;
	pthread_t				monitor_thread;
	const char				*digits;
	uint64_t				next_sequence;
	uint32_t				coders_started;
	bool					monitor_started;
	pthread_cond_t			start_cond;
	bool					simulation_started;

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

bool						acquire_both_dongles(t_coder *coder);
bool						acquire_dongle(t_coder *coder, t_dongle *dongle,
								int64_t cooldown);
bool						all_coders_finished(t_app *app);
void						*coder_routine(void *arg);
void						good_sleep(t_app *app, uint64_t ms_to_sleep);
bool						heap_push(t_heap *heap, t_request request,
								t_app *app);
t_request					*heap_peek(t_heap *heap);
void						heap_pop(t_heap *heap, t_app *app);
bool						heap_remove(t_heap *heap, t_coder *coder,
								t_app *app);
bool						is_stopped(t_app *app);
void						join_simulation(t_app *app);
void						log_msg_force(t_app *app, uint32_t id,
								const char *msg, int msg_len);
void						*monitor_routine(void *arg);
int64_t						now_ms(void);
void						build_deadline(struct timespec *ts,
								uint64_t ms_to_wait);
bool						request_before(t_app *app, const t_request *a,
								const t_request *b);
bool						scheduler_acquire_pair(t_coder *coder);
void						scheduler_dispatch(t_app *app);
bool						scheduler_is_best_ready(t_coder *coder);
bool						scheduler_pair_ready(t_coder *coder);

void						release_both_dongles(t_coder *coder);
void						release_dongle(t_dongle *dongle);
void						set_stop(t_app *app);
t_start_result				start_simulation(t_app *app);
bool						stop_for_burnout(t_app *app, uint32_t id);
void						wait_for_dongle(t_dongle *dongle,
								int64_t time_left);
t_init_result				init_fail_start_cond(t_app *app);
t_init_result				init_result_err(const char *msg);
bool						is_queue_head(t_dongle *dongle, t_coder *coder);
bool						wait_for_start(t_app *app);
void						set_all_coder_deadlines(t_app *app,
								int64_t start_time);
void						start_all_threads(t_app *app);

#endif
