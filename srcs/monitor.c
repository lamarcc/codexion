/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 02:51:40 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 04:59:25 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	broadcast_cond(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static void	set_end(t_simulation *sim)
{
	pthread_mutex_lock(&sim->mutex);
	sim->end_simulation = TRUE;
	pthread_mutex_unlock(&sim->mutex);
}

static int	burn_check(t_coder *coder, long burnout_time)
{
	long	check;

	pthread_mutex_lock(&coder->mutex);
	check = get_time(coder->sim) - coder->previous_compile;
	if (check > burnout_time && coder->nb_compile != coder->sim->nb_compile)
	{
		pthread_mutex_unlock(&coder->mutex);
		set_end(coder->sim);
		burn_log(coder->sim, coder->id);
		broadcast_cond(coder->sim);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	int				i;
	int				has_finished;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (TRUE)
	{
		i = 0;
		has_finished = 0;
		while (i < sim->nb_coders)
		{
			if (has_coder_finished(&sim->coders[i]))
				has_finished += 1;
			if (burn_check(&sim->coders[i], sim->burnout_time))
				return (NULL);
			i++;
		}
		if (has_finished == sim->nb_coders)
		{
			set_end(sim);
			return (NULL);
		}
	}
	return (NULL);
}
