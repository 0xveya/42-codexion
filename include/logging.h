/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   logging.h                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/10 22:26:03 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/10 22:43:25 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGING_H
# define LOGGING_H

# include <stdint.h>

typedef struct s_app	t_app;

typedef struct s_log_data
{
	int64_t				time;
	uint32_t			id;
	const char			*msg;
	int					msg_len;
	const char			*digits;
}						t_log_data;

# define MSG_FORK "has taken a dongle"
# define LEN_FORK 18

# define MSG_COMPILE "is compiling"
# define LEN_COMPILE 12

# define MSG_DEBUG "is debugging"
# define LEN_DEBUG 12

# define MSG_REFACTOR "is refactoring"
# define LEN_REFACTOR 14

# define MSG_BURNOUT "burned out"
# define LEN_BURNOUT 10

void					log_msg(t_app *app, uint32_t id, const char *msg,
							int msg_len);
void					log_msg_force(t_app *app, uint32_t id,
							const char *msg, int msg_len);

#endif
