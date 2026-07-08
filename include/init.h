/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.h                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:53:53 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 22:50:25 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H
# include "sim.h"

t_coder_init_result		init_coders(t_app *app);
t_dongle_init_result	init_dongles(uint32_t number_of_coders);
t_init_result			init_fail_coders(t_app *app, t_coder_init_result *c);
t_init_result			init_fail_dongles(t_app *app, t_dongle_init_result *d);
t_init_result			init_fail_heap(t_app *app, t_heap_init_result *h);
t_heap_init_result		init_heap(uint32_t capacity);
t_init_result			init_mutexes(t_app *app);
t_init_result			init_result_err(const char *msg);

#endif
