/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   scheduler_dispatch.c                              :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/15 23:00:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/15 23:00:00 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logging.h"
#include "../../include/sim.h"

/* Lock: caller holds state_mutex; this serializes the whole log group. */
static void	log_reservation(t_coder *coder)
{
	pthread_mutex_lock(&coder->app->log_mutex);
	log_msg_force(coder->app, coder->id, MSG_FORK, LEN_FORK);
	log_msg_force(coder->app, coder->id, MSG_FORK, LEN_FORK);
	log_msg_force(coder->app, coder->id, MSG_COMPILE, LEN_COMPILE);
	pthread_mutex_unlock(&coder->app->log_mutex);
}

/* Lock: caller must hold state_mutex. */
static void	reserve_pair(t_coder *coder)
{
	coder->left->available = false;
	coder->right->available = false;
	coder->last_compile_start = now_ms();
	coder->waiting = false;
	coder->granted = true;
	log_reservation(coder);
}

/* Lock: caller must hold state_mutex. */
void	scheduler_dispatch(t_app *app)
{
	uint32_t	i;
	bool		changed;

	changed = true;
	while (changed)
	{
		changed = false;
		i = 0;
		while (i < app->args.number_of_coders)
		{
			if (app->coders[i].waiting && now_ms()
				< app->coders[i].request.deadline
				&& scheduler_pair_ready(&app->coders[i])
				&& scheduler_is_best_ready(&app->coders[i]))
			{
				reserve_pair(&app->coders[i]);
				changed = true;
			}
			i++;
		}
	}
	pthread_cond_broadcast(&app->stop_cond);
}
