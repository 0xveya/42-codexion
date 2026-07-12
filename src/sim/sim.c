/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 19:41:50 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:24:46 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
#include "../../include/logging.h"
#include "../../include/sim.h"

/* Lock: holds state_mutex while reading compile counters. */
static int	coder_is_done(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->app->state_mutex);
	done = (coder->app->args.number_of_compiles_required > 0
			&& coder->compiles_done
			>= coder->app->args.number_of_compiles_required);
	pthread_mutex_unlock(&coder->app->state_mutex);
	return (done);
}

/* Lock: holds state_mutex while updating compile counters. */
static bool	finish_compile(t_coder *coder)
{
	bool	finished;

	finished = false;
	pthread_mutex_lock(&coder->app->state_mutex);
	coder->compiles_done++;
	if (coder->app->args.number_of_compiles_required > 0
		&& all_coders_finished(coder->app))
		finished = true;
	pthread_mutex_unlock(&coder->app->state_mutex);
	if (finished)
		set_stop(coder->app);
	return (finished);
}

/* Lock: holds state_mutex while updating last compile start. */
static void	compile_once(t_coder *coder)
{
	pthread_mutex_lock(&coder->app->state_mutex);
	coder->last_compile_start = now_ms();
	pthread_mutex_unlock(&coder->app->state_mutex);
	log_msg(coder->app, coder->id, MSG_COMPILE, LEN_COMPILE);
	good_sleep(coder->app, coder->app->args.time_to_compile);
	release_both_dongles(coder);
}

/* Lock: delegates locking to start, stop, acquire, log, and sleep helpers. */
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!wait_for_start(coder->app))
		return (NULL);
	while (!is_stopped(coder->app))
	{
		if (coder_is_done(coder) || !acquire_both_dongles(coder))
			break ;
		compile_once(coder);
		if (is_stopped(coder->app))
			break ;
		if (finish_compile(coder))
			break ;
		log_msg(coder->app, coder->id, MSG_DEBUG, LEN_DEBUG);
		good_sleep(coder->app, coder->app->args.time_to_debug);
		if (is_stopped(coder->app))
			break ;
		log_msg(coder->app, coder->id, MSG_REFACTOR, LEN_REFACTOR);
		good_sleep(coder->app, coder->app->args.time_to_refactor);
	}
	return (NULL);
}

/* Lock: caller must hold state_mutex. */
bool	all_coders_finished(t_app *app)
{
	uint32_t	i;

	i = 0;
	while (i < app->args.number_of_coders)
	{
		if (app->coders[i].compiles_done
			< app->args.number_of_compiles_required)
			return (false);
		i++;
	}
	return (true);
}
