/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 02:53:40 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	make_compile(t_coder *coder)
{
	if (take_dongle(coder))
		return (1);
	update_compile_time(coder);
	compile_log(coder, coder->id);
	usleep(coder->sim->compile_time * 1000);
	leave_dongle(coder);
	return (0);
}

static int	make_debug(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	debug_log(coder, coder->id);
	usleep(coder->sim->debug_time * 1000);
	return (0);
}

static int	make_refactor(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	refactor_log(coder, coder->id);
	usleep(coder->sim->refactor_time * 1000);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(coder->sim->dongle_cooldown * 1000);
	while (TRUE)
	{
		if (is_simulation_over(coder->sim))
			return (NULL);
		if (make_compile(coder))
			continue ;
		if (make_debug(coder))
			return (NULL);
		if (make_refactor(coder))
			return (NULL);
		if (has_coder_finished(coder))
			return (NULL);
	}
	return (NULL);
}
