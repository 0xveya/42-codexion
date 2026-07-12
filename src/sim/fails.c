/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   fails.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 22:49:18 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:50:47 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include "../../include/sim.h"
#include <stdlib.h>

/* Lock: none; builds an init error result. */
t_init_result	init_result_err(const char *msg)
{
	t_init_result	result;

	result.status = init_err;
	result.data.error_msg = msg;
	return (result);
}

/* Lock: destroys initialized app mutexes during failed startup. */
t_init_result	init_fail_dongles(t_app *app, t_dongle_init_result *d)
{
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	return (free(app), init_result_err(d->data.error_msg));
}

/* Lock: destroys initialized dongles and app mutexes. */
t_init_result	init_fail_coders(t_app *app, t_coder_init_result *c)
{
	free_dongles(app->dongles, app->args.number_of_coders);
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	return (free(app), init_result_err(c->data.error_msg));
}

/* Lock: destroys initialized resources during failed startup. */
t_init_result	init_fail_heap(t_app *app, t_heap_init_result *h)
{
	free_coders(app->coders, app->args.number_of_coders);
	free_dongles(app->dongles, app->args.number_of_coders);
	pthread_mutex_destroy(&app->state_mutex);
	pthread_mutex_destroy(&app->log_mutex);
	return (free(app), init_result_err(h->data.error_msg));
}

/* Lock: initializes app mutexes before threads exist. */
t_init_result	init_mutexes(t_app *app)
{
	t_init_result	result;

	if (pthread_mutex_init(&app->state_mutex, NULL) != 0)
		return (free(app), init_result_err("Failed to init state_mutex"));
	if (pthread_mutex_init(&app->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&app->state_mutex);
		return (free(app), init_result_err("Failed to init log_mutex"));
	}
	result.status = init_ok;
	return (result);
}
