/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   sim_cleanup.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:45:03 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 21:58:22 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"
#include <pthread.h>
#include <stdlib.h>

void	free_coders(t_coder *coders, uint32_t count)
{
	(void)count;
	free(coders);
}

void	free_dongles(t_dongle *dongles, uint32_t count)
{
	uint32_t	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	free(dongles);
}

void	free_heap(t_heap *heap)
{
	free(heap->data);
	heap->data = NULL;
}
