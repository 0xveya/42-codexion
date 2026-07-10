/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:41:50 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 00:31:10 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
#include "../../include/logging.h"
#include "../../include/sim.h"
#include <unistd.h>

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->app))
	{
		pthread_mutex_lock(&coder->app->state_mutex);
		if (coder->app->args.number_of_compiles_required > 0
			&& coder->compiles_done >= coder->app->args.number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->app->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->app->state_mutex);
		acquire_both_dongles(coder);
		if (is_stopped(coder->app))
		{
			release_both_dongles(coder);
			break ;
		}
		pthread_mutex_lock(&coder->app->state_mutex);
		coder->last_compile_start = now_ms();
		pthread_mutex_unlock(&coder->app->state_mutex);
		log_msg(coder->app, coder->id, MSG_COMPILE, LEN_COMPILE);
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
		log_msg(coder->app, coder->id, MSG_DEBUG, LEN_DEBUG);
		good_sleep(coder->app, coder->app->args.time_to_debug);
		if (is_stopped(coder->app))
			break ;
		log_msg(coder->app, coder->id, MSG_REFACTOR, LEN_REFACTOR);
		good_sleep(coder->app, coder->app->args.time_to_refactor);
		pthread_mutex_lock(&coder->app->state_mutex);
		coder->last_compile_start = now_ms();
		pthread_mutex_unlock(&coder->app->state_mutex);
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
					log_msg(app, coder->id, MSG_BURNOUT, LEN_BURNOUT);
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
