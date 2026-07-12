/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_helpers.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 18:59:59 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:43:25 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logging.h"
#include "../../include/sim.h"
#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>

/* Lock: none; reads wall clock. */
int64_t	now_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

/* Lock: holds state_mutex while reading simulation_stop. */
bool	is_stopped(t_app *app)
{
	bool	result;

	pthread_mutex_lock(&app->state_mutex);
	result = app->simulation_stop;
	pthread_mutex_unlock(&app->state_mutex);
	return (result);
}

/* Lock: locks each dongle mutex before broadcasting its condition. */
static void	wake_all_dongles(t_app *app)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		pthread_mutex_lock(&app->dongles[i].mutex);
		pthread_cond_broadcast(&app->dongles[i].cond);
		pthread_mutex_unlock(&app->dongles[i].mutex);
		i++;
	}
}

/* Lock: holds state_mutex, then wakes all dongle conditions. */
void	set_stop(t_app *app)
{
	pthread_mutex_lock(&app->state_mutex);
	app->simulation_stop = true;
	pthread_cond_broadcast(&app->stop_cond);
	pthread_cond_broadcast(&app->start_cond);
	pthread_mutex_unlock(&app->state_mutex);
	wake_all_dongles(app);
}

/* Lock: holds state_mutex and log_mutex for final burnout log. */
bool	stop_for_burnout(t_app *app, uint32_t id)
{
	pthread_mutex_lock(&app->state_mutex);
	pthread_mutex_lock(&app->log_mutex);
	if (app->simulation_stop)
	{
		pthread_mutex_unlock(&app->log_mutex);
		pthread_mutex_unlock(&app->state_mutex);
		return (false);
	}
	app->simulation_stop = true;
	pthread_cond_broadcast(&app->stop_cond);
	pthread_cond_broadcast(&app->start_cond);
	pthread_mutex_unlock(&app->state_mutex);
	log_msg_force(app, id, MSG_BURNOUT, LEN_BURNOUT);
	pthread_mutex_unlock(&app->log_mutex);
	wake_all_dongles(app);
	return (true);
}
