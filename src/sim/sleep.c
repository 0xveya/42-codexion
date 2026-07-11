/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sleep.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 23:10:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 23:10:00 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <sys/time.h>
#include <time.h>

void	build_deadline(struct timespec *ts, uint64_t ms_to_wait)
{
	struct timeval	tv;
	int64_t			target_us;

	gettimeofday(&tv, NULL);
	target_us = tv.tv_usec + (ms_to_wait * 1000);
	ts->tv_sec = tv.tv_sec + (target_us / 1000000);
	ts->tv_nsec = (target_us % 1000000) * 1000;
}

void	good_sleep(t_app *app, uint64_t ms_to_sleep)
{
	struct timespec	ts;

	if (ms_to_sleep == 0)
		return ;
	build_deadline(&ts, ms_to_sleep);
	pthread_mutex_lock(&app->state_mutex);
	while (!app->simulation_stop)
	{
		if (pthread_cond_timedwait(&app->stop_cond, &app->state_mutex,
				&ts) != 0)
			break ;
	}
	pthread_mutex_unlock(&app->state_mutex);
}
