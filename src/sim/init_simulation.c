/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init_simulation.c                                 :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:55:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 22:43:21 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include "../../include/logging.h"
#include <stdlib.h>

static t_init_result	init_components(t_app *app)
{
	t_dongle_init_result	dongles;
	t_coder_init_result		coders;
	t_init_result			result;

	dongles = init_dongles(app->args.number_of_coders);
	if (dongles.status == dongle_init_err)
		return (init_fail_dongles(app, &dongles));
	app->dongles = dongles.data.success;
	coders = init_coders(app);
	if (coders.status == coder_init_err)
		return (init_fail_coders(app, &coders));
	app->coders = coders.data.success;
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
	pthread_cond_init(&app->stop_cond, NULL);
	link_dongles(app);
	app->digits = "00010203040506070809"
					"10111213141516171819"
					"20212223242526272829"
					"30313233343536373839"
					"40414243444546474849"
					"50515253545556575859"
					"60616263646566676869"
					"70717273747576777879"
					"80818283848586878889"
					"90919293949596979899";
	result.status = init_ok;
	result.data.success = app;
	return (result);
}
