/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   utils.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/07 20:14:10 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/07 20:40:33 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/args.h"

/* Lock: none; pure character helper. */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/* Lock: none; pure integer parser. */
t_int_result	ft_parse_uint(const char *str, int is_u32)
{
	t_int_result	res;
	int				i;

	res.status = int_err;
	res.value = 0;
	if (!str || !str[0])
		return (res);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (res);
		if (res.value > (UINT64_MAX - (str[i] - '0')) / 10)
			return (res);
		res.value = res.value * 10 + (str[i] - '0');
		i++;
	}
	if (is_u32 && res.value > UINT32_MAX)
		return (res);
	return (res.status = int_ok, res);
}
