/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   dongle_init.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:54:30 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 21:54:32 by sfurst          ###   ########.fr        */
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

t_dongle_init_result	init_dongles(uint32_t number_of_coders)
{
	t_dongle_init_result	result;

	result.data.success = malloc(sizeof(t_dongle) * number_of_coders);
	if (result.data.success == NULL)
		return (dongle_alloc_err());
	result.status = dongle_init_ok;
	return (result);
}
