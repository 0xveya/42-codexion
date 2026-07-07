/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   args.h                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:06:18 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/07 20:49:13 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

# include <stddef.h>
# include <stdint.h>

typedef enum e_field_type
{
	type_u32,
	type_u64
}						t_field_type;

typedef struct s_arg_field
{
	t_field_type		type;
	size_t				offset;
}						t_arg_field;

typedef enum e_scheduler_policy
{
	scheduler_fifo,
	scheduler_edf
}						t_scheduler_policy;

typedef struct s_fifo_data
{
	uint8_t				reserved;
}						t_fifo_data;

typedef struct s_edf_data
{
	uint8_t				reserved;
}						t_edf_data;

typedef union u_policy_data
{
	t_fifo_data			fifo_data;
	t_edf_data			edf_data;
}						t_policy_data;

typedef struct s_scheduler_arg
{
	t_scheduler_policy	policy;
	t_policy_data		data;
}						t_scheduler_arg;

typedef struct s_args
{
	uint32_t			number_of_coders;
	uint64_t			time_to_burnout;
	uint64_t			time_to_compile;
	uint64_t			time_to_debug;
	uint64_t			time_to_refactor;
	uint32_t			number_of_compiles_required;
	uint64_t			dongle_cooldown;
	t_scheduler_arg		scheduler;
}						t_args;

typedef struct s_field_desc
{
	size_t				offset;
	size_t				size;
}						t_field_desc;

typedef enum e_parse_status
{
	parse_ok,
	parse_err
}						t_parse_status;

typedef union u_parse_data
{
	t_args				success;
	const char			*error_msg;
}						t_parse_data;

typedef struct s_parse_result
{
	t_parse_status		status;
	t_parse_data		data;
}						t_parse_result;

typedef enum e_int_status
{
	int_ok,
	int_err
}						t_int_status;

typedef struct s_int_result
{
	t_int_status		status;
	uint64_t			value;
}						t_int_result;

t_parse_result			parse_arguments(int argc, char *argv[]);

void					free_arguments(t_args *args);
t_int_result			ft_parse_uint(const char *str, int is_u32);

#endif
