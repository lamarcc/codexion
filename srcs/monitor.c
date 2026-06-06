/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/06 04:43:48 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	*check_burnout(void *arg)
{
	int				i;
	int				j;
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
				has_finished += 1;
			if (has_finished == sim->nb_coders)
			{
				pthread_mutex_unlock(&sim->coders[i].mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->coders[i].mutex);
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = get_time(sim) - sim->coders[i].previous_compile;
			if (burnout_check > sim->burnout_time && !sim->coders[i].finished)
			{
				pthread_mutex_unlock(&sim->coders[i].mutex);
				burn_log(sim, sim->coders[i].id);
				pthread_mutex_lock(&sim->mutex_sim);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex_sim);
				j = 0;
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
