/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_helpers.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 18:59:59 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 22:32:52 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <sys/time.h>

int64_t	now_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

bool	is_stopped(t_app *app)
{
	bool	result;

	pthread_mutex_lock(&app->state_mutex);
	result = app->simulation_stop;
	pthread_mutex_unlock(&app->state_mutex);
	return (result);
}
