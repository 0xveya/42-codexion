/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   monitor.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 00:38:54 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 21:26:54 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <unistd.h>

static bool	check_coder_burnout(t_app *app, t_coder *coder)
{
	int64_t	elapsed;

	if (coder->last_compile_start <= 0)
		return (false);
	elapsed = now_ms() - coder->last_compile_start;
	if (elapsed < (int64_t)app->args.time_to_burnout)
		return (false);
	pthread_mutex_unlock(&app->state_mutex);
	if (stop_for_burnout(app, coder->id))
		return (true);
	pthread_mutex_lock(&app->state_mutex);
	return (false);
}

static bool	check_burnouts(t_app *app)
{
	uint32_t	i;

	pthread_mutex_lock(&app->state_mutex);
	i = 0;
	while (i < app->args.number_of_coders)
	{
		if (check_coder_burnout(app, &app->coders[i]))
			return (true);
		i++;
	}
	pthread_mutex_unlock(&app->state_mutex);
	return (false);
}

void	*monitor_routine(void *arg)
{
	t_app	*app;

	app = (t_app *)arg;
	while (!is_stopped(app))
	{
		if (check_burnouts(app))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
