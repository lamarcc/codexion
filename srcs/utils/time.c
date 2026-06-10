/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:03:54 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 05:24:18 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	start_time(t_simulation *sim)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&sim->mutex);
	sim->start_time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	pthread_mutex_unlock(&sim->mutex);
}

void	update_compile_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->nb_compile++;
	coder->previous_compile = get_time(coder->sim);
	pthread_mutex_unlock(&coder->mutex);
}

long	get_time(t_simulation *sim)
{
	long			time;
	struct timeval	t;

	gettimeofday(&t, NULL);
	time = (t.tv_sec * 1000) + (t.tv_usec / 1000) - sim->start_time;
	return (time);
}

long	time_since(t_simulation *sim, long time)
{
	return (get_time(sim) - time);
}

int	d_cooldown(t_simulation *sim, t_dongle *dongle)
{
	return (time_since(sim, dongle->last_released + sim->dongle_cooldown) >= 0);
}
