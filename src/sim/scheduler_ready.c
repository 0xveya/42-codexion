/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_ready.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfurst <sfurst@student.42vienna.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 00:00:00 by sfurst           #+#    #+#             */
/*   Updated: 2026/07/13 00:00:00 by sfurst          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

/* Lock: caller must hold state_mutex. */
static bool	dongle_ready(t_coder *coder, t_dongle *dongle)
{
	int64_t	ready_at;

	ready_at = dongle->released_at + coder->app->args.dongle_cooldown;
	return (dongle->available && now_ms() >= ready_at);
}

/* Lock: caller must hold state_mutex. */
bool	scheduler_pair_ready(t_coder *coder)
{
	return (dongle_ready(coder, coder->left)
		&& dongle_ready(coder, coder->right));
}

/* Lock: caller must hold state_mutex. */
bool	scheduler_is_best_ready(t_coder *coder)
{
	t_app		*app;
	uint32_t	i;
	t_coder		*other;

	app = coder->app;
	i = 0;
	while (i < app->args.number_of_coders)
	{
		other = &app->coders[i++];
		if (other != coder && other->waiting
			&& scheduler_pair_ready(other)
			&& request_before(app, &other->request, &coder->request))
			return (false);
	}
	return (true);
}
