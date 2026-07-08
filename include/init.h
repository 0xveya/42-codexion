/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.h                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: sfurst <sfurst@student.42vienna.com>      #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/07/08 21:53:53 by sfurst           #+#    #+#              */
/*   Updated: 2026/07/08 21:53:59 by sfurst          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H
# include "sim.h"

t_coder_init_result		init_coders(t_app *app);
t_dongle_init_result	init_dongles(uint32_t number_of_coders);
t_heap_init_result		init_heap(uint32_t capacity);

#endif
