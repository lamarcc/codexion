/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/06 02:44:38 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	*check_burnout(void *arg)
{
	int				i;
	int				j;
	int				has_finished;
	long			time;
	long			burnout_check;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	has_finished = 0;
	while (TRUE)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_mutex_lock(&sim->coders[i].mutex);
			if (sim->coders[i].finished)
				has_finished += 1;
			if (has_finished == sim->nb_coders)
			{
				pthread_mutex_unlock(&sim->coders[i].mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->coders[i].mutex);
			time = get_time(sim);
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = time - sim->coders[i].previous_compile;
			pthread_mutex_unlock(&sim->coders[i].mutex);
			pthread_mutex_lock(&sim->coders[i].mutex);
			if (burnout_check > sim->burnout_time && !sim->coders[i].finished)
			{
				pthread_mutex_unlock(&sim->coders[i].mutex);
				j = 0;
				burn_log(sim, sim->coders[i].id, get_time(sim));
				pthread_mutex_lock(&sim->mutex_sim);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex_sim);
				while (j < sim->nb_coders)
				{
					pthread_mutex_lock(&sim->dongles[j].mutex);
					pthread_cond_broadcast(&sim->dongles[j].cond);
					pthread_mutex_unlock(&sim->dongles[j].mutex);
					j++;
				}
				return (NULL);
			}
			pthread_mutex_unlock(&sim->coders[i].mutex);
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
