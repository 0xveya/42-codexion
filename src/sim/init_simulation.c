/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init_simulation.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:55:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 21:55:12 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"

static t_init_result	init_result_err(const char *msg)
{
	t_init_result	result;

	result.status = init_err;
	result.data.error_msg = msg;
	return (result);
}

static t_init_result	init_fail_dongles(t_dongle_init_result *d)
{
	return (init_result_err(d->data.error_msg));
}

static t_init_result	init_fail_coders(t_app *app, t_coder_init_result *c)
{
	free_dongles(app->dongles, app->args.number_of_coders);
	return (init_result_err(c->data.error_msg));
}

static t_init_result	init_fail_heap(t_app *app, t_heap_init_result *h)
{
	free_coders(app->coders, app->args.number_of_coders);
	free_dongles(app->dongles, app->args.number_of_coders);
	return (init_result_err(h->data.error_msg));
}

t_init_result	init_simulation(const t_args *args)
{
	t_dongle_init_result	dongles;
	t_coder_init_result		coders;
	t_heap_init_result		heap;
	t_init_result			result;
	t_app					app;

	app.args = *args;
	dongles = init_dongles(args->number_of_coders);
	if (dongles.status == dongle_init_err)
		return (init_fail_dongles(&dongles));
	app.dongles = dongles.data.success;
	coders = init_coders(&app);
	if (coders.status == coder_init_err)
		return (init_fail_coders(&app, &coders));
	app.coders = coders.data.success;
	heap = init_heap(args->number_of_coders);
	if (heap.status == heap_init_err)
		return (init_fail_heap(&app, &heap));
	app.sched_heap = heap.data.success;
	result.status = init_ok;
	result.data.success = app;
	return (result);
}
