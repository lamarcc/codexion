/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 05:24:16 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	smart_sleep(int ms, t_simulation *sim)
{
	long	time;
	int		sim_ready;

	time = get_time(sim);
	while (time + ms > get_time(sim))
	{
		pthread_mutex_lock(&sim->mutex);
		sim_ready = sim->end_simulation;
		pthread_mutex_unlock(&sim->mutex);
		if (sim_ready)
			return (1);
		usleep(1000);
	}
	return (0);
}

static int	make_compile(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	if (take_dongle(coder))
		return (1);
	update_compile_time(coder);
	compile_log(coder, coder->id);
	if (smart_sleep(coder->sim->compile_time, coder->sim))
		return (1);
	leave_dongle(coder);
	return (0);
}

static int	make_debug(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	debug_log(coder, coder->id);
	if (smart_sleep(coder->sim->debug_time, coder->sim))
		return (1);
	return (0);
}

static int	make_refactor(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	refactor_log(coder, coder->id);
	if (smart_sleep(coder->sim->refactor_time, coder->sim))
		return (1);
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
			return (NULL) ;
		if (make_debug(coder))
			return (NULL);
		if (make_refactor(coder))
			return (NULL);
		if (has_coder_finished(coder))
			return (NULL);
	}
	return (NULL);
}
