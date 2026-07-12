/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_pop.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 23:10:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 23:10:00 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

/* Lock: caller protects the heap being mutated. */
static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Lock: caller protects the heap being inspected. */
static uint32_t	best_child(t_heap *heap, uint32_t index, t_app *app)
{
	uint32_t	left;
	uint32_t	right;
	uint32_t	best;

	left = index * 2 + 1;
	right = left + 1;
	best = index;
	if (left < heap->size && request_before(app, &heap->data[left],
			&heap->data[best]))
		best = left;
	if (right < heap->size && request_before(app, &heap->data[right],
			&heap->data[best]))
		best = right;
	return (best);
}

/* Lock: caller must hold dongle mutex for shared heaps. */
void	heap_pop(t_heap *heap, t_app *app)
{
	uint32_t	index;
	uint32_t	best;

	if (heap->size == 0)
		return ;
	heap->size--;
	heap->data[0] = heap->data[heap->size];
	index = 0;
	while (index < heap->size)
	{
		best = best_child(heap, index, app);
		if (best == index)
			break ;
		swap_requests(&heap->data[index], &heap->data[best]);
		index = best;
	}
}
