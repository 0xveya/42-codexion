/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   acquire.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 22:50:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:11:14 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logging.h"
#include "../../include/sim.h"

/* Lock: none; computes deterministic dongle order. */
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

/* Lock: uses acquire_dongle and release_dongle helpers. */
static bool	acquire_single_coder_dongle(t_coder *coder)
{
	if (!acquire_dongle(coder, coder->left, coder->app->args.dongle_cooldown))
		return (false);
	log_msg(coder->app, coder->id, MSG_FORK, LEN_FORK);
	good_sleep(coder->app, coder->app->args.time_to_burnout + 10);
	release_dongle(coder->left);
	return (false);
}

/* Lock: uses acquire_dongle and releases partial acquisition on failure. */
bool	acquire_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int64_t		cooldown;

	if (coder->left == coder->right)
		return (acquire_single_coder_dongle(coder));
	ordered_pair(coder, &first, &second);
	cooldown = coder->app->args.dongle_cooldown;
	if (!acquire_dongle(coder, first, cooldown))
		return (false);
	log_msg(coder->app, coder->id, MSG_FORK, LEN_FORK);
	if (!acquire_dongle(coder, second, cooldown))
	{
		release_dongle(first);
		return (false);
	}
	log_msg(coder->app, coder->id, MSG_FORK, LEN_FORK);
	return (true);
}

/* Lock: caller must hold dongle mutex while waiting on dongle cond. */
void	wait_for_dongle(t_dongle *dongle, int64_t time_left)
{
	struct timespec	wait_deadline;

	if (time_left == 0)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		return ;
	}
	if (time_left > 2000)
		time_left = 2000;
	build_deadline(&wait_deadline, (uint64_t)time_left);
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &wait_deadline);
}
