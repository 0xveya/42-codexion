/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   scheduler_pair.c                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/13 00:00:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/15 22:28:38 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logging.h"
#include "../../include/sim.h"

/* Lock: caller must hold state_mutex. */
static void	register_request(t_coder *coder)
{
	coder->request.sequence = coder->app->next_sequence++;
	coder->request.deadline = coder->last_compile_start
		+ coder->app->args.time_to_burnout;
	coder->waiting = true;
	coder->granted = false;
}

/* Lock: caller must hold state_mutex. */
static int64_t	pair_cooldown_left(t_coder *coder)
{
	int64_t	left_at;
	int64_t	right_at;
	int64_t	ready_at;

	left_at = coder->left->released_at
		+ (int64_t)coder->app->args.dongle_cooldown;
	right_at = coder->right->released_at
		+ (int64_t)coder->app->args.dongle_cooldown;
	ready_at = left_at;
	if (right_at > ready_at)
		ready_at = right_at;
	if (ready_at > coder->request.deadline)
		ready_at = coder->request.deadline;
	return (ready_at - now_ms());
}

/* Lock: caller must hold state_mutex. */
static void	wait_for_scheduler(t_coder *coder)
{
	struct timespec	deadline;
	int64_t			wait_ms;

	wait_ms = coder->request.deadline - now_ms();
	if (coder->left->available && coder->right->available)
		wait_ms = pair_cooldown_left(coder);
	if (wait_ms < 1)
		wait_ms = 1;
	build_deadline(&deadline, (uint64_t)wait_ms);
	pthread_cond_timedwait(&coder->app->stop_cond, &coder->app->state_mutex,
		&deadline);
}

/* Lock: holds state_mutex while scheduling both dongles atomically. */
bool	scheduler_acquire_pair(t_coder *coder)
{
	pthread_mutex_lock(&coder->app->state_mutex);
	register_request(coder);
	while (!coder->app->simulation_stop)
	{
		if (now_ms() >= coder->request.deadline)
			break ;
		scheduler_dispatch(coder->app);
		if (coder->granted)
		{
			pthread_mutex_unlock(&coder->app->state_mutex);
			return (true);
		}
		wait_for_scheduler(coder);
	}
	coder->waiting = false;
	coder->granted = false;
	pthread_cond_broadcast(&coder->app->stop_cond);
	pthread_mutex_unlock(&coder->app->state_mutex);
	return (false);
}
