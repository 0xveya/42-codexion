/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_helpers.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 18:59:59 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 19:23:19 by sfurst          ###   ########.fr        */
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

void	log_msg(t_app *app, uint32_t coder_id, const char *msg)
{
	int64_t	elapsed;

	pthread_mutex_lock(&app->log_mutex);
	elapsed = now_ms() - app->start_time;
	printf("%ld %u %s\n", (long)elapsed, coder_id, msg);
	pthread_mutex_unlock(&app->log_mutex);
}

bool	is_stopped(t_app *app)
{
	bool	result;

	pthread_mutex_lock(&app->state_mutex);
	result = app->simulation_stop;
	pthread_mutex_unlock(&app->state_mutex);
	return (result);
}
