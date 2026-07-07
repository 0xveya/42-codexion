/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:14:15 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/07 20:44:40 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/args.h"
#include <stdio.h>

int	main(int argc, char *argv[])
{
	t_parse_result	res;

	res = parse_arguments(argc, argv);
	if (res.status == parse_err)
	{
		return (fprintf(stderr, "%s\n", res.data.error_msg), 1);
	}
	printf("number_of_coders: %u\n", res.data.success.number_of_coders);
	printf("time_to_burnout: %lu\n", res.data.success.time_to_burnout);
	printf("time_to_compile: %lu\n", res.data.success.time_to_compile);
	printf("time_to_debug: %lu\n", res.data.success.time_to_debug);
	printf("time_to_refactor: %lu\n", res.data.success.time_to_refactor);
	printf("number_of_compiles_required: %u\n",
		res.data.success.number_of_compiles_required);
	printf("dongle_cooldown: %lu\n", res.data.success.dongle_cooldown);
	if (res.data.success.scheduler.policy == scheduler_fifo)
		printf("scheduler: fifo\n");
	else if (res.data.success.scheduler.policy == scheduler_edf)
		printf("scheduler: edf\n");
	return (0);
}
