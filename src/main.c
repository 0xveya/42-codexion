/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:14:15 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:40:07 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/args.h"
#include "../include/sim.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	debug_print_app(t_app *app)
{
	uint32_t	i;

	printf("=== dongles (%u) ===\n", app->args.number_of_coders);
	i = 0;
	while (i < app->args.number_of_coders)
	{
		printf("dongle[%u] available=%d released_at=%ld\n", i,
			app->dongles[i].available, app->dongles[i].released_at);
		i++;
	}
	printf("=== coders (%u) ===\n", app->args.number_of_coders);
	i = 0;
	while (i < app->args.number_of_coders)
	{
		printf("coder[%u] id=%u state=%d left=%p right=%p\n", i,
			app->coders[i].id, app->coders[i].state,
			(void *)app->coders[i].left, (void *)app->coders[i].right);
		i++;
	}
	printf("=== heap === size=%u capacity=%u\n", app->sched_heap.size,
		app->sched_heap.capacity);
}

static void	cleanup_app(t_app *app)
{
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	free_coders(app->coders, app->args.number_of_coders);
	free_dongles(app->dongles, app->args.number_of_coders);
	free_heap(&app->sched_heap);
	free(app);
}

static void	stop_simulation(t_app *app)
{
	pthread_mutex_lock(&app->state_mutex);
	app->simulation_stop = true;
	pthread_mutex_unlock(&app->state_mutex);
}

int	main(int argc, char *argv[])
{
	t_parse_result	res;
	t_init_result	init_res;
	t_start_result	start_res;
	t_app			*app;

	res = parse_arguments(argc, argv);
	if (res.status == parse_err)
	{
		fprintf(stderr, "%s\n", res.data.error_msg);
		return (1);
	}
	init_res = init_simulation(&res.data.success);
	if (init_res.status == init_err)
	{
		fprintf(stderr, "%s\n", init_res.data.error_msg);
		return (1);
	}
	app = init_res.data.success;
	debug_print_app(app);
	start_res = start_simulation(app);
	if (start_res.status == sim_start_err)
	{
		fprintf(stderr, "%s\n", start_res.data.error_msg);
		cleanup_app(app);
		return (1);
	}
	printf("threads started, letting sim run briefly...\n");
	sleep(1);
	stop_simulation(app);
	join_simulation(app);
	printf("threads joined cleanly\n");
	cleanup_app(app);
	printf("cleanup done\n");
	return (0);
}
