/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_pair.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:00:00 by sfurst           #+#    #+#             */
/*   Updated: 2026/07/13 00:00:00 by sfurst          ###   ########.fr       */
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
}

/* Lock: caller must hold state_mutex. */
static void	wait_for_scheduler(t_coder *coder)
{
	struct timespec	deadline;
	int64_t			left_at;
	int64_t			right_at;
	int64_t			wait_ms;

	if (!coder->left->available || !coder->right->available)
	{
		pthread_cond_wait(&coder->app->stop_cond,
			&coder->app->state_mutex);
		return ;
	}
	left_at = coder->left->released_at + coder->app->args.dongle_cooldown;
	right_at = coder->right->released_at + coder->app->args.dongle_cooldown;
	wait_ms = left_at;
	if (right_at > wait_ms)
		wait_ms = right_at;
	wait_ms -= now_ms();
	if (wait_ms < 1)
		wait_ms = 1;
	build_deadline(&deadline, wait_ms);
	pthread_cond_timedwait(&coder->app->stop_cond,
		&coder->app->state_mutex, &deadline);
}

/* Lock: caller must hold state_mutex. */
static void	reserve_pair(t_coder *coder)
{
	coder->left->available = false;
	coder->right->available = false;
	coder->waiting = false;
	pthread_cond_broadcast(&coder->app->stop_cond);
}

/* Lock: holds state_mutex while scheduling both dongles atomically. */
bool	scheduler_acquire_pair(t_coder *coder)
{
	pthread_mutex_lock(&coder->app->state_mutex);
	register_request(coder);
	while (!coder->app->simulation_stop)
	{
		if (scheduler_pair_ready(coder)
			&& scheduler_is_best_ready(coder))
		{
			reserve_pair(coder);
			pthread_mutex_unlock(&coder->app->state_mutex);
			log_msg(coder->app, coder->id, MSG_FORK, LEN_FORK);
			log_msg(coder->app, coder->id, MSG_FORK, LEN_FORK);
			return (true);
		}
		wait_for_scheduler(coder);
	}
	coder->waiting = false;
	pthread_cond_broadcast(&coder->app->stop_cond);
	pthread_mutex_unlock(&coder->app->state_mutex);
	return (false);
}
