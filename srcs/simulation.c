/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/01 05:51:01 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	compile(t_coder *coder)
{
	if (take_dongle(coder))
		return (1);
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	update_compile_time(coder);
	printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
	printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
	printf("%ld %d is compiling\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->compile_time * 1000);
	leave_dongle(coder);
	return (0);
}

int	debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex_mon);
	if (coder->sim->end_simulation)
	{
		pthread_mutex_unlock(&coder->sim->mutex_mon);
		return (1);
	}
	pthread_mutex_unlock(&coder->sim->mutex_mon);
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	printf("%ld %d is debugging\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->debug_time * 1000);
	return (0);
}

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex_mon);
	if (coder->sim->end_simulation)
	{
		pthread_mutex_unlock(&coder->sim->mutex_mon);
		return (1);
	}
	pthread_mutex_unlock(&coder->sim->mutex_mon);
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	printf("%ld %d is refactoring\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->refactor_time * 1000);
	return (0);
}

void	*routine(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	while (!coder->sim->end_simulation)
	{
		if (compile(coder))
		{
			leave_dongle(coder);
			return (NULL);
		}
		if (debug(coder))
			return (NULL);
		if (refactor(coder))
			return (NULL);
		pthread_mutex_lock(&coder->sim->mutex);
		coder->nb_compile += 1;
		pthread_mutex_unlock(&coder->sim->mutex);
		pthread_mutex_lock(&coder->mutex);
		if (coder->nb_compile == coder->sim->nb_compile)
		{
			pthread_mutex_unlock(&coder->mutex);
			pthread_mutex_lock(&coder->sim->mutex);
			coder->finished = TRUE;
			pthread_mutex_unlock(&coder->sim->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&coder->mutex);
	}
	return (NULL);
}

void	*check_burnout(void *arg)
{
	int				i;
	int				j;
	int				has_finished;
	long			time;
	long			burnout_check;
	t_simulation	*sim;
	struct timeval	monitor_t;

	sim = (t_simulation *)arg;
	has_finished = 0;
	while (TRUE)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_mutex_lock(&sim->mutex);
			if (sim->coders[i].finished)
				has_finished += 1;
			if (has_finished == sim->nb_coders)
			{
				pthread_mutex_unlock(&sim->mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->mutex);
			gettimeofday(&monitor_t, NULL);
			time = (monitor_t.tv_sec * 1000) + (monitor_t.tv_usec / 1000);
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = (time - sim->start_time) - sim->coders[i].previous_compile;
			pthread_mutex_unlock(&sim->coders[i].mutex);
			if (burnout_check > sim->burnout_time && sim->coders[i].previous_compile > -1)
			{
				j = 0;
				pthread_mutex_lock(&sim->mutex_mon);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex_mon);
				while (j < sim->nb_coders)
				{
					pthread_mutex_lock(&sim->dongles[j].mutex);
					pthread_cond_broadcast(&sim->dongles[j].cond);
					pthread_mutex_unlock(&sim->dongles[j].mutex);
					j++;
				}
				pthread_mutex_lock(&sim->mutex);
				printf("%ld %d burned out\n", sim->time, sim->coders[i].id);
				pthread_mutex_unlock(&sim->mutex);
				return (NULL);
			}
			i++;
		}
	}
}

int	run(t_simulation *sim)
{
	int	i;

	start_time(sim);
	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, &routine, &sim->coders[i]) != 0)
			cleanup(sim);
		usleep(10);
		i++;
	}
	usleep(1000);
	if (pthread_create(&sim->monitor, NULL, &check_burnout, sim) != 0)
		cleanup(sim);
	return (1);
}
