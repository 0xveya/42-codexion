/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   start_deadline.c                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/11 23:10:00 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 23:10:00 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/sim.h"

void	set_initial_deadlines(t_app *app)
{
	uint32_t	i;

	app->start_time = now_ms();
	i = 0;
	while (i < app->args.number_of_coders)
	{
		app->coders[i].last_compile_start = app->start_time;
		i++;
	}
}
