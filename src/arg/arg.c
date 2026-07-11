/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   arg.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:16:32 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/11 22:34:47 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
#include <stdint.h>
#include <string.h>

static inline int	parse_scheduler(t_args *args, const char *str)
{
	if (strcmp(str, "fifo") == 0)
	{
		args->scheduler.policy = scheduler_fifo;
		return (args->scheduler.data.fifo_data.reserved = 0, 1);
	}
	else if (strcmp(str, "edf") == 0)
	{
		args->scheduler.policy = scheduler_edf;
		return (args->scheduler.data.edf_data.reserved = 0, 1);
	}
	return (0);
}

static void	init_fields_map(t_field_desc fields[7])
{
	fields[0] = (t_field_desc){offsetof(t_args, number_of_coders),
		sizeof(uint32_t)};
	fields[1] = (t_field_desc){offsetof(t_args, time_to_burnout),
		sizeof(uint64_t)};
	fields[2] = (t_field_desc){offsetof(t_args, time_to_compile),
		sizeof(uint64_t)};
	fields[3] = (t_field_desc){offsetof(t_args, time_to_debug),
		sizeof(uint64_t)};
	fields[4] = (t_field_desc){offsetof(t_args, time_to_refactor),
		sizeof(uint64_t)};
	fields[5] = (t_field_desc){offsetof(t_args, number_of_compiles_required),
		sizeof(uint32_t)};
	fields[6] = (t_field_desc){offsetof(t_args, dongle_cooldown),
		sizeof(uint64_t)};
}

static int	validate_semantic_constraints(const t_args *args)
{
	if (args->number_of_coders == 0)
		return (0);
	if (args->number_of_compiles_required == 0)
		return (0);
	if (args->time_to_burnout > (uint64_t)(INT64_MAX / 1000)
		|| args->time_to_compile > (uint64_t)(INT64_MAX / 1000)
		|| args->time_to_debug > (uint64_t)(INT64_MAX / 1000)
		|| args->time_to_refactor > (uint64_t)(INT64_MAX / 1000)
		|| args->dongle_cooldown > (uint64_t)(INT64_MAX / 1000))
	{
		return (0);
	}
	return (1);
}

static t_parse_result	parse_values(t_args *args, char *argv[])
{
	t_int_result	parsed;
	t_field_desc	fields[7];
	void			*dest;
	int				i;

	init_fields_map(fields);
	i = -1;
	while (++i < 7)
	{
		dest = (char *)args + fields[i].offset;
		parsed = ft_parse_uint(argv[i + 1],
				fields[i].size == sizeof(uint32_t));
		if (parsed.status == int_err)
			return ((t_parse_result){parse_err, {.error_msg = "Error: Int"}});
		if (fields[i].size == sizeof(uint32_t))
			*(uint32_t *)dest = (uint32_t)parsed.value;
		else
			*(uint64_t *)dest = parsed.value;
	}
	return ((t_parse_result){parse_ok, {.success = *args}});
}

t_parse_result	parse_arguments(int argc, char *argv[])
{
	t_parse_result	res;

	if (argc != 9)
		return ((t_parse_result){parse_err, {.error_msg = "Error: Args"}});
	res = parse_values(&res.data.success, argv);
	if (res.status == parse_err)
		return (res);
	if (!parse_scheduler(&res.data.success, argv[8]))
		return ((t_parse_result){parse_err, {.error_msg = "Error: Policy"}});
	if (!validate_semantic_constraints(&res.data.success))
		return ((t_parse_result){parse_err,
			{.error_msg = "Error: Invalid Values"}});
	return (res.status = parse_ok, res);
}
