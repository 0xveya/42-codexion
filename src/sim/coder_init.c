/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   coder_init.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:54:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 00:28:32 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/init.h"
#include <stdlib.h>

static void	init_coder_fields(t_coder *coder, t_app *app, uint32_t id)
{
	coder->id = id;
	coder->app = app;
	coder->state = taking_dongle;
	coder->last_compile_start = now_ms();
	coder->compiles_done = 0;
	coder->left = NULL;
	coder->right = NULL;
	coder->thread = 0;
}

static t_coder_init_result	coder_alloc_err(void)
{
	t_coder_init_result	result;

	result.status = coder_init_err;
	result.data.error_msg = "Failed to allocate coders";
	return (result);
}

t_coder_init_result	init_coders(t_app *app)
{
	t_coder_init_result	result;
	uint32_t			i;

	result.data.success = malloc(sizeof(t_coder) * app->args.number_of_coders);
	if (result.data.success == NULL)
		return (coder_alloc_err());
	i = 0;
	while (i < app->args.number_of_coders)
	{
		init_coder_fields(&result.data.success[i], app, i + 1);
		i++;
	}
	result.status = coder_init_ok;
	return (result);
}
