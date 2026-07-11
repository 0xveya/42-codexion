/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   printing.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 22:24:44 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 23:12:33 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/logging.h"
#include "../../include/sim.h"
#include <unistd.h>

static inline void	copy_bytes(char *dst, const char *src, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

static inline int	append_u64(char *dst, uint64_t n, const char *digits)
{
	char		tmp[20];
	int			pos;
	uint64_t	part;

	pos = 20;
	while (n >= 100)
	{
		part = (n % 100) * 2;
		pos -= 2;
		tmp[pos] = digits[part];
		tmp[pos + 1] = digits[part + 1];
		n /= 100;
	}
	if (n < 10)
		tmp[--pos] = n + '0';
	else
	{
		part = n * 2;
		pos -= 2;
		tmp[pos] = digits[part];
		tmp[pos + 1] = digits[part + 1];
	}
	copy_bytes(dst, tmp + pos, 20 - pos);
	return (20 - pos);
}

static int	build_line(char *buf, t_log_data *data)
{
	int	len;

	len = 0;
	len += append_u64(buf + len, data->time, data->digits);
	buf[len++] = ' ';
	len += append_u64(buf + len, data->id, data->digits);
	buf[len++] = ' ';
	copy_bytes(buf + len, data->msg, data->msg_len);
	len += data->msg_len;
	buf[len++] = '\n';
	return (len);
}

void	log_msg_force(t_app *app, uint32_t id, const char *msg, int msg_len)
{
	char		buf[64];
	int			len;
	t_log_data	data;

	data.time = now_ms() - app->start_time;
	data.id = id;
	data.msg = msg;
	data.msg_len = msg_len;
	data.digits = app->digits;
	len = build_line(buf, &data);
	write(1, buf, len);
}

void	log_msg(t_app *app, uint32_t id, const char *msg, int msg_len)
{
	pthread_mutex_lock(&app->state_mutex);
	if (app->simulation_stop)
	{
		pthread_mutex_unlock(&app->state_mutex);
		return ;
	}
	pthread_mutex_lock(&app->log_mutex);
	pthread_mutex_unlock(&app->state_mutex);
	log_msg_force(app, id, msg, msg_len);
	pthread_mutex_unlock(&app->log_mutex);
}

/*
void	log_msg(t_app *app, uint32_t id, const char *msg, int msg_len)
{
	int64_t	elapsed;

	(void)msg_len;
	elapsed = now_ms() - app->start_time;
	pthread_mutex_lock(&app->log_mutex);
	printf("%ld %u %s\n", (long)elapsed, id, msg);
	pthread_mutex_unlock(&app->log_mutex);
}
*/
