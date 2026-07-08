/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:41:50 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 22:18:28 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
#include "../../include/sim.h"
#include <unistd.h>

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->app))
	{
		acquire_both_dongles(coder);
		if (is_stopped(coder->app))
		{
			release_both_dongles(coder);
			break ;
		}
		pthread_mutex_lock(&coder->app->state_mutex);
		coder->last_compile_start = now_ms();
		pthread_mutex_unlock(&coder->app->state_mutex);
		log_msg(coder->app, coder->id, "is compiling");
		good_sleep(coder->app, coder->app->args.time_to_compile);
		release_both_dongles(coder);
		if (is_stopped(coder->app))
			break ;
		pthread_mutex_lock(&coder->app->state_mutex);
		coder->compiles_done++;
		if (coder->app->args.number_of_compiles_required > 0
			&& all_coders_finished(coder->app))
		{
			coder->app->simulation_stop = true;
			pthread_cond_broadcast(&coder->app->stop_cond);
		}
		pthread_mutex_unlock(&coder->app->state_mutex);
		log_msg(coder->app, coder->id, "is debugging");
		good_sleep(coder->app, coder->app->args.time_to_debug);
		if (is_stopped(coder->app))
			break ;
		log_msg(coder->app, coder->id, "is refactoring");
		good_sleep(coder->app, coder->app->args.time_to_refactor);
	}
	return (NULL);
}
void	set_stop(t_app *app)
{
	pthread_mutex_lock(&app->state_mutex);
	app->simulation_stop = true;
	pthread_cond_broadcast(&app->stop_cond);
	pthread_mutex_unlock(&app->state_mutex);
}

bool	all_coders_finished(t_app *app)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		if (app->coders[i].compiles_done < app->args.number_of_compiles_required)
			return (false);
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_app		*app;
	t_coder		*coder;
	int64_t		elapsed;
	uint32_t	i;

	app = (t_app *)arg;
	while (!is_stopped(app))
	{
		pthread_mutex_lock(&app->state_mutex);
		i = 0;
		while (i < app->args.number_of_coders)
		{
			coder = &app->coders[i];
			if (coder->last_compile_start > 0)
			{
				elapsed = now_ms() - coder->last_compile_start;
				if (elapsed >= (int64_t)app->args.time_to_burnout)
				{
					pthread_mutex_unlock(&app->state_mutex);
					log_msg(app, coder->id, "burned out");
					set_stop(app);
					return (NULL);
				}
			}
			i++;
		}
		pthread_mutex_unlock(&app->state_mutex);
		usleep(1000);
	}
	return (NULL);
}
