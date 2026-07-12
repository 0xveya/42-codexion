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
	while (app->coders_started > 0)
	{
		app->coders_started--;
		pthread_join(app->coders[app->coders_started].thread, NULL);
	}
	if (app->monitor_started)
	{
		pthread_join(app->monitor_thread, NULL);
		app->monitor_started = false;
	}
}
