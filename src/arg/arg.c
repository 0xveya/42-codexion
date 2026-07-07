/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   arg.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:16:32 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/07 20:49:53 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"
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

t_parse_result	parse_arguments(int argc, char *argv[])
{
	t_parse_result	res;
	t_int_result	parsed;
	t_field_desc	fields[7];
	void			*dest;
	int				i;

	if (argc != 9)
		return ((t_parse_result){parse_err, {.error_msg = "Error: Args"}});
	init_fields_map(fields);
	i = -1;
	while (++i < 7)
	{
		dest = (char *)&res.data.success + fields[i].offset;
		parsed = ft_parse_uint(argv[i + 1], fields[i].size == sizeof(uint32_t));
		if (parsed.status == int_err)
			return ((t_parse_result){parse_err, {.error_msg = "Error: Int"}});
		if (fields[i].size == sizeof(uint32_t))
			*(uint32_t *)dest = (uint32_t)parsed.value;
		else
			*(uint64_t *)dest = parsed.value;
	}
	if (!parse_scheduler(&res.data.success, argv[8]))
		return ((t_parse_result){parse_err, {.error_msg = "Error: Policy"}});
	return (res.status = parse_ok, res);
}
