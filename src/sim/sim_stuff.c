/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_stuff.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 19:08:17 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 21:36:32 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

static int64_t	dongle_time_left(t_dongle *dongle, int64_t cooldown)
{
	int64_t	time_left;
	int64_t	current;
	int64_t	wake_at;

	current = now_ms();
	wake_at = dongle->released_at + cooldown;
	if (dongle->available && current >= wake_at)
		return (0);
	if (!dongle->available)
		time_left = 2000;
	else
		time_left = wake_at - current;
	if (time_left <= 0)
		time_left = 1;
	return (time_left);
}

static int64_t	get_coder_deadline(t_coder *coder)
{
	int64_t	deadline;

	pthread_mutex_lock(&coder->app->state_mutex);
	deadline = coder->last_compile_start
		+ coder->app->args.time_to_burnout;
	pthread_mutex_unlock(&coder->app->state_mutex);
	return (deadline);
}

static bool	push_request(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	request.coder = coder;
	request.deadline = get_coder_deadline(coder);
	request.sequence = dongle->next_sequence++;
	return (heap_push(&dongle->queue, request, coder->app));
}

static bool	acquire_if_ready(t_coder *coder, t_dongle *dongle,
		int64_t time_left)
{
	t_request	*head;

	head = heap_peek(&dongle->queue);
	if (head == NULL || head->coder != coder || time_left != 0)
		return (false);
	heap_pop(&dongle->queue, coder->app);
	dongle->available = false;
	pthread_cond_broadcast(&dongle->cond);
	return (true);
}

bool	acquire_dongle(t_coder *coder, t_dongle *dongle, int64_t cooldown)
{
	struct timespec	wait_deadline;
	int64_t			time_left;

	pthread_mutex_lock(&dongle->mutex);
	if (!push_request(coder, dongle))
		return (pthread_mutex_unlock(&dongle->mutex), false);
	while (!is_stopped(coder->app))
	{
		time_left = dongle_time_left(dongle, cooldown);
		if (acquire_if_ready(coder, dongle, time_left))
			return (pthread_mutex_unlock(&dongle->mutex), true);
		if (time_left > 2000)
			time_left = 2000;
		build_deadline(&wait_deadline, (uint64_t)time_left);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex,
			&wait_deadline);
	}
	heap_remove(&dongle->queue, coder, coder->app);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (false);
}
