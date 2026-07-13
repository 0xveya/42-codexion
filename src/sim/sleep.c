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
#include <time.h>
#include <unistd.h>

/* Lock: none; builds an absolute condition timeout. */
void	build_deadline(struct timespec *ts, uint64_t ms_to_wait)
{
	uint64_t	target_ns;

	clock_gettime(CLOCK_REALTIME, ts);
	target_ns = (uint64_t)ts->tv_nsec + (ms_to_wait * 1000000);
	ts->tv_sec += target_ns / 1000000000;
	ts->tv_nsec = target_ns % 1000000000;
}

/* Lock: stop checks lock state_mutex; short sleeps keep shutdown responsive. */
void	good_sleep(t_app *app, uint64_t ms_to_sleep)
{
	int64_t	end;
	int64_t	left;

	if (ms_to_sleep == 0)
		return ;
	end = now_ms() + (int64_t)ms_to_sleep;
	while (!is_stopped(app))
	{
		left = end - now_ms();
		if (left <= 0)
			break ;
		if (left > 1)
			left = 1;
		usleep((useconds_t)left * 1000);
	}
}
