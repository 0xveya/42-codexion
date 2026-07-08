/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   thread_start.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 22:27:47 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 21:14:25 by sfurst          ###   ########.fr        */
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

static t_start_result	start_coders(t_app *app, uint32_t created)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		if (pthread_create(&app->coders[i].thread, NULL, coder_routine,
				&app->coders[i]) != 0)
		{
			set_stop(app);
			while (created--)
				pthread_join(app->coders[created].thread, NULL);
			return (make_start_err("Failed to create coder thread"));
		}
		created++;
		i++;
	}
	return (start_ok_result());
}

t_start_result	start_simulation(t_app *app)
{
	t_start_result	result;

	app->start_time = now_ms();
	app->simulation_stop = false;
	if (pthread_create(&app->monitor_thread, NULL, monitor_routine, app) != 0)
	{
		set_stop(app);
		join_simulation(app);
		return (make_start_err("Failed to create monitor thread"));
	}
	result = start_coders(app, 0);
	if (result.status == sim_start_err)
		return (result);
	return (start_ok_result());
}
