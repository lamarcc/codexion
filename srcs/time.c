/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:03:54 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/07 07:01:12 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	start_time(t_simulation *sim)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&sim->mutex_log);
	sim->start_time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	pthread_mutex_unlock(&sim->mutex_log);
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

void	update_compile_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->previous_compile = get_time(coder->sim);
	pthread_mutex_unlock(&coder->mutex);
}

void	update_dongle_time(t_simulation *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->last_released = get_time(sim);
	pthread_mutex_unlock(&dongle->mutex);
}
