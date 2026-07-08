/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:14:15 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 21:57:42 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/args.h"
#include "../include/sim.h"
#include <stdio.h>

void	debug_print_app(t_app *app)
{
	uint32_t	i;

	printf("=== args ===\n");
	printf("number_of_coders: %u\n", app->args.number_of_coders);
	printf("time_to_burnout: %lu\n", app->args.time_to_burnout);
	printf("time_to_compile: %lu\n", app->args.time_to_compile);
	printf("time_to_debug: %lu\n", app->args.time_to_debug);
	printf("time_to_refactor: %lu\n", app->args.time_to_refactor);
	printf("compiles_required: %u\n", app->args.number_of_compiles_required);
	printf("dongle_cooldown: %lu\n", app->args.dongle_cooldown);
	printf("scheduler: %s\n",
		app->args.scheduler.policy == scheduler_fifo ? "fifo" : "edf");
	printf("=== dongles (%u) ===\n", app->args.number_of_coders);
	i = 0;
	while (i < app->args.number_of_coders)
	{
		printf("dongle[%u] available=%d released_at=%ld\n", i,
			app->dongles[i].available, app->dongles[i].released_at);
		i++;
	}
	printf("=== coders (%u) ===\n", app->args.number_of_coders);
	i = 0;
	while (i < app->args.number_of_coders)
	{
		printf("coder[%u] id=%u state=%d left=%p right=%p\n", i,
			app->coders[i].id, app->coders[i].state,
			(void *)app->coders[i].left, (void *)app->coders[i].right);
		i++;
	}
	printf("=== heap ===\n");
	printf("size=%u capacity=%u data=%p\n", app->sched_heap.size,
		app->sched_heap.capacity, (void *)app->sched_heap.data);
}

int	main(int argc, char *argv[])
{
	t_parse_result	res;
	t_init_result	init_res;

	res = parse_arguments(argc, argv);
	if (res.status == parse_err)
	{
		return (fprintf(stderr, "%s\n", res.data.error_msg), 1);
	}
	init_res = init_simulation(&res.data.success);
	if (init_res.status == init_err)
		return (fprintf(stderr, "%s\n", init_res.data.error_msg), 1);
	debug_print_app(&init_res.data.success);
	printf("success initing i guess\n");
	return (0);
}
