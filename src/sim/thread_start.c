/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   thread_start.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 22:27:47 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:28:16 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <stdint.h>

static t_start_result	make_start_err(const char *msg)
{
	t_start_result	result;

	result.status = sim_start_err;
	result.data.error_msg = msg;
	return (result);
}

static t_start_result	start_ok_result(void)
{
	t_start_result	result;

	result.status = sim_start_ok;
	result.data.dummy = 0;
	return (result);
}

static t_start_result	start_coders(t_app *app)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		if (pthread_create(&app->coders[i].thread, NULL, coder_routine,
				&app->coders[i]) != 0)
		{
			return (make_start_err("Failed to create coder thread"));
		}
		app->coders_started++;
		i++;
	}
	return (start_ok_result());
}

static void	join_started_threads(t_app *app)
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

t_start_result	start_simulation(t_app *app)
{
	t_start_result	result;

	app->simulation_stop = false;
	app->simulation_started = false;
	app->coders_started = 0;
	app->monitor_started = false;
	if (pthread_create(&app->monitor_thread, NULL, monitor_routine, app) != 0)
	{
		set_stop(app);
		return (make_start_err("Failed to create monitor thread"));
	}
	app->monitor_started = true;
	result = start_coders(app);
	if (result.status == sim_start_err)
	{
		set_stop(app);
		join_started_threads(app);
		return (result);
	}
	start_all_threads(app);
	return (start_ok_result());
}
