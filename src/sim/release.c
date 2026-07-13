/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   release.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 19:22:06 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 21:38:06 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

/* Lock: holds dongle mutex and broadcasts dongle condition. */
void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = true;
	dongle->released_at = now_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

/* Lock: delegates to release_dongle for each held dongle. */
void	release_both_dongles(t_coder *coder)
{
	int64_t	released_at;

	released_at = now_ms();
	pthread_mutex_lock(&coder->app->state_mutex);
	coder->left->available = true;
	coder->left->released_at = released_at;
	coder->right->available = true;
	coder->right->released_at = released_at;
	pthread_cond_broadcast(&coder->app->stop_cond);
	pthread_mutex_unlock(&coder->app->state_mutex);
}
