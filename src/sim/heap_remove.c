/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_remove.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 23:10:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 00:59:38 by sfurst          ###   ########.fr        */
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

static void	sift_up(t_heap *heap, uint32_t index, t_app *app)
{
	uint32_t	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(app, &heap->data[index], &heap->data[parent]))
			break ;
		swap_requests(&heap->data[index], &heap->data[parent]);
		index = parent;
	}
}

static void	sift_down(t_heap *heap, uint32_t index, t_app *app)
{
	uint32_t	child;
	uint32_t	right;

	while (index * 2 + 1 < heap->size)
	{
		child = index * 2 + 1;
		right = child + 1;
		if (right < heap->size && request_before(app, &heap->data[right],
				&heap->data[child]))
			child = right;
		if (!request_before(app, &heap->data[child], &heap->data[index]))
			break ;
		swap_requests(&heap->data[index], &heap->data[child]);
		index = child;
	}
}

bool	heap_remove(t_heap *heap, t_coder *coder, t_app *app)
{
	uint32_t	i;
	uint32_t	parent;

	i = 0;
	while (i < heap->size && heap->data[i].coder != coder)
		i++;
	if (i == heap->size)
		return (false);
	heap->size--;
	if (i == heap->size)
		return (true);
	heap->data[i] = heap->data[heap->size];
	if (i > 0)
	{
		parent = (i - 1) / 2;
		if (request_before(app, &heap->data[i], &heap->data[parent]))
		{
			sift_up(heap, i, app);
			return (true);
		}
	}
	sift_down(heap, i, app);
	return (true);
}
