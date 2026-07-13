/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   start_helpers.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/12 01:21:20 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:42:29 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <stdlib.h>

/* Lock: holds state_mutex while waiting on start_cond. */
bool	wait_for_start(t_app *app)
{
	pthread_mutex_lock(&app->state_mutex);
	while (!app->simulation_started && !app->simulation_stop)
		pthread_cond_wait(&app->start_cond, &app->state_mutex);
	if (app->simulation_stop)
	{
		pthread_mutex_unlock(&app->state_mutex);
		return (false);
	}
	pthread_mutex_unlock(&app->state_mutex);
	return (true);
}

/* Lock: destroys initialized synchronization objects on init failure. */
t_init_result	init_fail_start_cond(t_app *app)
{
	pthread_cond_destroy(&app->stop_cond);
	free_coders(app->coders, app->args.number_of_coders);
	free_dongles(app->dongles, app->args.number_of_coders);
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	free(app);
	return (init_result_err("Failed to init start_cond"));
}

/* Lock: caller must hold state_mutex. */
void	set_all_coder_deadlines(t_app *app, int64_t start_time)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		app->coders[i].last_compile_start = start_time;
		i++;
	}
}

/* Lock: holds state_mutex and broadcasts start_cond. */
void	start_all_threads(t_app *app)
{
	pthread_mutex_lock(&app->state_mutex);
	app->start_time = now_ms();
	app->next_sequence = 0;
	set_all_coder_deadlines(app, app->start_time);
	app->simulation_started = true;
	pthread_cond_broadcast(&app->start_cond);
	pthread_mutex_unlock(&app->state_mutex);
}
