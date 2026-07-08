/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   dongle_init.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:54:30 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:37:06 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include <stdlib.h>

static t_dongle_init_result	dongle_alloc_err(void)
{
	t_dongle_init_result	result;

	result.status = dongle_init_err;
	result.data.error_msg = "Failed to allocate dongles";
	return (result);
}

static t_dongle_init_result	dongle_init_err_result(t_dongle *dongles,
		uint32_t initialized)
{
	t_dongle_init_result	result;
	uint32_t				i;

	i = 0;
	while (i < initialized)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	free(dongles);
	result.status = dongle_init_err;
	result.data.error_msg = "Failed to init dongle mutex/cond";
	return (result);
}

static bool	init_one_dongle(t_dongle *dongle)
{
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (false);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (false);
	}
	dongle->available = true;
	dongle->released_at = 0;
	return (true);
}

t_dongle_init_result	init_dongles(uint32_t number_of_coders)
{
	t_dongle_init_result	result;
	t_dongle				*dongles;
	uint32_t				i;

	dongles = malloc(sizeof(t_dongle) * number_of_coders);
	if (dongles == NULL)
		return (dongle_alloc_err());
	i = 0;
	while (i < number_of_coders)
	{
		if (!init_one_dongle(&dongles[i]))
			return (dongle_init_err_result(dongles, i));
		i++;
	}
	result.status = dongle_init_ok;
	result.data.success = dongles;
	return (result);
}
