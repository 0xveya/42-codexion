/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_push.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 23:10:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 23:10:00 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

bool	heap_push(t_heap *heap, t_request request, t_app *app)
{
	uint32_t	index;
	uint32_t	parent;

	if (heap->size >= heap->capacity)
		return (false);
	index = heap->size;
	heap->data[index] = request;
	heap->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(app, &heap->data[index], &heap->data[parent]))
			break ;
		swap_requests(&heap->data[index], &heap->data[parent]);
		index = parent;
	}
	return (true);
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->data[0]);
}
