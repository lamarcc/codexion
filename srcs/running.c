/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/09 05:49:14 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	*check_burnout(void *arg)
{
	int				i;
	int				has_finished;
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
			{
				if (has_finished == sim->nb_coders)
				{
					pthread_mutex_unlock(&sim->coders[i].mutex);
					return (NULL);
				}
				has_finished += 1;
			}
			pthread_mutex_unlock(&sim->coders[i].mutex);
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = get_time(sim) - sim->coders[i].previous_compile;
			if (burnout_check > sim->burnout_time && !sim->coders[i].finished)
			{
				pthread_mutex_unlock(&sim->coders[i].mutex);
				pthread_mutex_lock(&sim->mutex);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex);
				burn_log(sim, sim->coders[i].id);
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
