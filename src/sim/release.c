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
	release_dongle(coder->left);
	if (coder->right != coder->left)
		release_dongle(coder->right);
}
