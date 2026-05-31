/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/31 06:03:28 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	compile(t_coder *coder)
{
	take_dongle(coder);
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	update_compile_time(coder);
	printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
	printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
	printf("%ld %d is compiling\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->compile_time * 1000);
	leave_dongle(coder);
}

void	debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	printf("%ld %d is debugging\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->debug_time * 1000);
}

void	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	update_time(coder->sim);
	printf("%ld %d is refactoring\n", coder->sim->time, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->refactor_time * 1000);
}

void	*routine(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	while (!coder->sim->end_simulation)
	{
		compile(coder);
		debug(coder);
		refactor(coder);
		pthread_mutex_lock(&coder->sim->mutex);
		coder->nb_compile += 1;
		pthread_mutex_unlock(&coder->sim->mutex);
		usleep(coder->sim->dongle_cooldown);
	}
	return (NULL);
}

void	*check_burnout(void *arg)
{
	int				i;
	long			time;
	long			burnout_check;
	struct timeval	monitor_t;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (TRUE)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = sim->coders[i].previous_compile + sim->burnout_time;
			pthread_mutex_unlock(&sim->coders[i].mutex);
			gettimeofday(&monitor_t, NULL);
			time = (monitor_t.tv_sec * 1000) + (monitor_t.tv_usec / 1000);
			if (time > burnout_check)
			{
				pthread_mutex_lock(&sim->mutex_mon);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex_mon);
				printf("%ld %d has burned out", time, sim->coders[i].id);
				cleanup(sim);
			}
			usleep(1000);
			i++;
		}
	}
}

int	run(t_simulation *sim)
{
	int	i;

	start_time(sim);
	i = 0;
	if (pthread_create(&sim->monitor, NULL, &check_burnout, sim) != 0)
		cleanup(sim);
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, &routine, &sim->coders[i]) != 0)
			cleanup(sim);
		usleep(10);
		i++;
	}
	return (1);
}
