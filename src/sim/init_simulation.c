/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init_simulation.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:55:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:50:03 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include <stdlib.h>

static t_init_result	init_components(t_app *app)
{
	t_dongle_init_result	dongles;
	t_coder_init_result		coders;
	t_heap_init_result		heap;
	t_init_result			result;

	dongles = init_dongles(app->args.number_of_coders);
	if (dongles.status == dongle_init_err)
		return (init_fail_dongles(app, &dongles));
	app->dongles = dongles.data.success;
	coders = init_coders(app);
	if (coders.status == coder_init_err)
		return (init_fail_coders(app, &coders));
	app->coders = coders.data.success;
	heap = init_heap(app->args.number_of_coders);
	if (heap.status == heap_init_err)
		return (init_fail_heap(app, &heap));
	app->sched_heap = heap.data.success;
	result.status = init_ok;
	return (result);
}

static void	link_dongles(t_app *app)
{
	uint32_t	i;
	uint32_t	n;

	i = 0;
	n = app->args.number_of_coders;
	while (i < n)
	{
		app->coders[i].left = &app->dongles[i];
		app->coders[i].right = &app->dongles[(i + 1) % n];
		i++;
	}
}

t_init_result	init_simulation(const t_args *args)
{
	t_app			*app;
	t_init_result	result;

	app = malloc(sizeof(t_app));
	if (app == NULL)
		return (init_result_err("Failed to allocate app"));
	app->args = *args;
	result = init_mutexes(app);
	if (result.status == init_err)
		return (result);
	result = init_components(app);
	if (result.status == init_err)
		return (result);
	link_dongles(app);
	result.status = init_ok;
	result.data.success = app;
	return (result);
}
