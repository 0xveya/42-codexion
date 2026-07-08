/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:41:50 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:46:54 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
#include "../../include/sim.h"
#include <stdio.h>
#include <unistd.h>

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	bool	stop;

	coder = (t_coder *)arg;
	stop = false;
	while (!stop)
	{
		pthread_mutex_lock(&coder->app->state_mutex);
		stop = coder->app->simulation_stop;
		pthread_mutex_unlock(&coder->app->state_mutex);
		if (!stop)
			usleep(1000);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_app		*app;
	bool		stop;
	uint32_t	i;

	app = (t_app *)arg;
	stop = false;
	while (!stop)
	{
		pthread_mutex_lock(&app->state_mutex);
		stop = app->simulation_stop;
		if (!stop)
		{
			printf("[Monitor] Status Check:\n");
			i = 0;
			while (i < app->args.number_of_coders)
			{
				printf("  Coder %u: state=%d compiles=%u\n", app->coders[i].id,
					app->coders[i].state, app->coders[i].compiles_done);
				i++;
			}
		}
		pthread_mutex_unlock(&app->state_mutex);
		usleep(200000);
	}
	return (NULL);
}
