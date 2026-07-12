/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   thread_join.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 22:29:07 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:29:09 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

/* Lock: none directly; waits for already-started threads to exit. */
void	join_simulation(t_app *app)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		pthread_join(app->coders[i].thread, NULL);
		i++;
	}
	pthread_join(app->monitor_thread, NULL);
}
