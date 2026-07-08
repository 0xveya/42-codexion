/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_stuff.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 19:08:17 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 19:59:28 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <sys/time.h>
#include <time.h>

static void	ordered_pair(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left < coder->right)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

static void	build_deadline(struct timespec *ts, uint64_t ms_to_wait)
{
	struct timeval	tv;
	int64_t			target_us;

	gettimeofday(&tv, NULL);
	target_us = tv.tv_usec + (ms_to_wait * 1000);
	ts->tv_sec = tv.tv_sec + (target_us / 1000000);
	ts->tv_nsec = (target_us % 1000000) * 1000;
}

void	acquire_dongle(t_coder *coder, t_dongle *dongle, int64_t cooldown)
{
	struct timespec	wait_deadline;

	pthread_mutex_lock(&dongle->mutex);
	while ((!dongle->available || now_ms() < dongle->released_at + cooldown)
		&& !is_stopped(coder->app))
	{
		build_deadline(&wait_deadline, 2000);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &wait_deadline);
	}
	if (!is_stopped(coder->app))
		dongle->available = false;
	pthread_mutex_unlock(&dongle->mutex);
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

void	acquire_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int64_t		cooldown;

	if (coder->left == coder->right)
	{
		acquire_dongle(coder, coder->left, coder->app->args.dongle_cooldown);
		log_msg(coder->app, coder->id, "has taken a dongle");
		good_sleep(coder->app, coder->app->args.time_to_burnout + 10);
		return ;
	}
	ordered_pair(coder, &first, &second);
	cooldown = coder->app->args.dongle_cooldown;
	acquire_dongle(coder, first, cooldown);
	if (is_stopped(coder->app))
		return ;
	log_msg(coder->app, coder->id, "has taken a dongle");
	acquire_dongle(coder, second, cooldown);
	if (is_stopped(coder->app))
		return ;
	log_msg(coder->app, coder->id, "has taken a dongle");
}
