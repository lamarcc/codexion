/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 03:42:28 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/28 05:24:22 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].taken = 0;
		sim->dongles[i].last_released = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].nb_compile = 0;
		sim->coders[i].previous_compile_start = 0;
		if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
			return (1);
		sim->coders[i].left_d = &sim->dongles[i];
		sim->coders[i].right_d = &sim->dongles[(i + 1) % sim->nb_coders];
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

int	initialize(t_simulation *sim, char **args)
{
	sim->nb_coders = atoi(args[1]);
	sim->time_burnout = atoi(args[2]);
	sim->time_compile = atoi(args[3]);
	sim->time_debug = atoi(args[4]);
	sim->time_refactor = atoi(args[5]);
	sim->nb_compile = atoi(args[6]);
	sim->dongle_cooldown = atoi(args[7]);
	if (strcmp("fifo", args[8]) == 0)
		sim->scheduler = TRUE;
	else
		sim->scheduler = FALSE;
	if (pthread_mutex_init(&sim->mutex_log, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->mutex_sim, NULL) != 0)
		return (1);
	sim->coders = calloc(sim->nb_coders, sizeof(t_coder));
	sim->dongles = calloc(sim->nb_coders, sizeof(t_dongle));
	if (init_dongles(sim))
		return (1);
	if (init_coders(sim))
		return (1);
	int i = 0;
	while (i < sim->nb_coders)
	{
		if (i%2==0)
		{
			sim->dongles[i].first = &sim->coders[i];
			if (sim->coders[(i + 1) % sim->nb_coders].id != 0)
				sim->dongles[i].second = &sim->coders[i + 1];
		}
		else
		{
			sim->dongles[i].first = &sim->coders[(i+1)%sim->nb_coders];
			if (sim->coders[(i + 1) % sim->nb_coders].id != 0)
				sim->dongles[i].second = &sim->coders[i];
		}
		i++;
	}
	return (0);
}
