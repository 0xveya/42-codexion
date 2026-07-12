/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   heap_helpers.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/12 01:13:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/12 01:13:35 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

/* Lock: caller must hold dongle mutex protecting the heap. */
bool	is_queue_head(t_dongle *dongle, t_coder *coder)
{
	t_request	*head;

	head = heap_peek(&dongle->queue);
	if (head == NULL)
		return (false);
	return (head->coder == coder);
}
