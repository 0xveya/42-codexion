/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:14:15 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 22:22:55 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/args.h"
#include "../include/sim.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	cleanup_app(t_app *app)
{
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	pthread_cond_destroy(&app->stop_cond);
	free_coders(app->coders, app->args.number_of_coders);
	free_dongles(app->dongles, app->args.number_of_coders);
	free(app);
}

static int	init_app(int argc, char *argv[], t_app **app)
{
	t_parse_result	res;
	t_init_result	init_res;

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
	*app = init_res.data.success;
	return (0);
}

int	main(int argc, char *argv[])
{
	t_start_result	start_res;
	t_app			*app;

	if (init_app(argc, argv, &app) != 0)
		return (1);
	start_res = start_simulation(app);
	if (start_res.status == sim_start_err)
	{
		fprintf(stderr, "%s\n", start_res.data.error_msg);
		cleanup_app(app);
		return (1);
	}
	join_simulation(app);
	cleanup_app(app);
	return (0);
}
