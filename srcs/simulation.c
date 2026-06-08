/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/08 04:09:12 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	compile(t_coder *coder)
{
	if (take_dongle(coder))
		return (1);
	update_compile_time(coder);
	compile_log(coder, coder->id);
	usleep(coder->sim->compile_time * 1000);
	leave_dongle(coder);
	return (0);
}

int	debug(t_coder *coder)
{
	if (is_simulation_over(coder->sim))
		return (1);
	debug_log(coder, coder->id);
	usleep(coder->sim->debug_time * 1000);
	return (0);
}

int	refactor(t_coder *coder)
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
	while (!is_simulation_over(coder->sim))
	{
		if (is_simulation_over(coder->sim))
			return (NULL);
		if (compile(coder))
			continue ;
		pthread_mutex_lock(&coder->mutex);
		coder->nb_compile += 1;
		pthread_mutex_unlock(&coder->mutex);
		has_coder_finished(coder);
		if (debug(coder))
			return (NULL);
		if (refactor(coder))
			return (NULL);
		if (has_coder_finished(coder))
			return (NULL);
	}
	return (NULL);
}

static void	check_priority(t_coder *coder, t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&coder->mutex);
	pthread_mutex_lock(&left->left->mutex);
	pthread_mutex_lock(&right->right->mutex);
	if (coder->previous_compile < left->left->previous_compile && coder->previous_compile < right->right->previous_compile)
	{
		pthread_mutex_lock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
		left->priority = coder;
		right->priority = coder;
		pthread_mutex_unlock(&left->mutex);
		pthread_mutex_unlock(&right->mutex);
	}
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_unlock(&left->left->mutex);
	pthread_mutex_unlock(&right->right->mutex);
}

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
			{
				if (has_finished == sim->nb_coders)
				{
					pthread_mutex_unlock(&sim->coders[i].mutex);
					return (NULL);
				}
				has_finished += 1;
			}
			pthread_mutex_unlock(&sim->coders[i].mutex);
			if (!sim->scheduler)
				check_priority(&sim->coders[i], sim->coders[i].left_d, sim->coders[i].right_d);
			pthread_mutex_lock(&sim->coders[i].mutex);
			burnout_check = get_time(sim) - sim->coders[i].previous_compile;
			if (burnout_check > sim->burnout_time && !sim->coders[i].finished)
			{
				pthread_mutex_lock(&sim->mutex_sim);
				sim->end_simulation = TRUE;
				pthread_mutex_unlock(&sim->mutex_sim);
				pthread_mutex_unlock(&sim->coders[i].mutex);
				burn_log(sim, sim->coders[i].id);
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
