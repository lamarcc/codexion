/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 04:23:31 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/09 05:43:32 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

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

void	*routine(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (TRUE)
	{
		if (is_simulation_over(coder->sim))
			return (NULL);
		if (make_compile(coder))
			continue ;
		pthread_mutex_lock(&coder->mutex);
		coder->nb_compile += 1;
		pthread_mutex_unlock(&coder->mutex);
		has_coder_finished(coder);
		if (make_debug(coder))
			return (NULL);
		if (make_refactor(coder))
			return (NULL);
		if (has_coder_finished(coder))
			return (NULL);
	}
	return (NULL);
}
