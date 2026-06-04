/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:03:54 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/05 00:00:50 by celamarc         ###   ########lyon.fr   */
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

void	update_time(t_simulation *sim)
{
	long			sec;
	long			usec;
	struct timeval	t;

	gettimeofday(&t, NULL);
	sec = t.tv_sec * 1000;
	usec = t.tv_usec / 1000;
	pthread_mutex_lock(&sim->mutex_log);
	sim->time = (sec + usec) - sim->start_time;
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

void	update_compile_time(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->sim->mutex_log);
	time = coder->sim->time;
	pthread_mutex_unlock(&coder->sim->mutex_log);
	pthread_mutex_lock(&coder->mutex);
	coder->previous_compile = time;
	pthread_mutex_unlock(&coder->mutex);
}

void	update_dongle_time(long start_time, t_dongle *dongle)
{
	long			time;
	struct timeval	t;

	pthread_mutex_lock(&dongle->mutex);
	gettimeofday(&t, NULL);
	time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	dongle->last_released = time - start_time;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
