/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_init.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:54:52 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 21:02:57 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include <stdlib.h>

static t_heap_init_result	heap_capacity_err(void)
{
	t_heap_init_result	result;

	result.status = heap_init_err;
	result.data.error_msg = "Capacity must be greater than 0";
	return (result);
}

static t_heap_init_result	heap_alloc_err(void)
{
	t_heap_init_result	result;

	result.status = heap_init_err;
	result.data.error_msg = "Failed to allocate heap";
	return (result);
}

t_heap_init_result	init_heap(uint32_t capacity)
{
	t_heap_init_result	result;

	if (capacity == 0)
		return (heap_capacity_err());
	result.data.success.size = 0;
	result.data.success.capacity = capacity;
	result.data.success.data = malloc(sizeof(t_coder *) * capacity);
	if (result.data.success.data == NULL)
		return (heap_alloc_err());
	result.status = heap_init_ok;
	return (result);
}
