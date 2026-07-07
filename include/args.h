/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   args.h                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:06:18 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/07 20:13:26 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

# include "qol.h"

typedef enum e_scheduler_policy
{
	scheduler_fifo,
	scheduler_edf
}						t_scheduler_policy;

typedef struct s_scheduler_arg
{
	t_scheduler_policy	policy;
	union				u_policy_data
	{
		struct			s_fifo_data
		{
			U8			reserved;
		} fifo_data;
		struct			s_edf_data
		{
			U8			reserved;
		} edf_data;
	} data;
}						t_scheduler_arg;

typedef struct s_args
{
	U32					number_of_coders;
	U64					time_to_burnout;
	U64					time_to_compile;
	U64					time_to_debug;
	U64					time_to_refactor;
	U32					number_of_compiles_required;
	U64					dongle_cooldown;
	t_scheduler_arg		scheduler;
}						t_args;

typedef enum e_parse_status
{
	parse_ok,
	parse_err
}						t_parse_status;

typedef struct s_parse_result
{
	t_parse_status		status;
	union				u_parse_data
	{
		t_args			success;
		const char		*error_msg;
	} data;
}						t_parse_result;

t_parse_result			parse_arguments(int argc, char *argv[]);

void					free_arguments(t_args *args);

#endif
