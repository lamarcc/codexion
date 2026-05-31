/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 03:42:28 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/01 00:37:15 by celamarc         ###   ########lyon.fr   */
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
		sim->dongles[i].taken = FALSE;
		sim->dongles[i].last_released = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (1);
		sim->dongles[i].left = NULL;
		sim->dongles[i].right = NULL;
		sim->dongles[i].queue[0] = NULL;
		sim->dongles[i].queue[1] = NULL;
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
		sim->coders[i].has_dongle = FALSE;
		sim->coders[i].nb_compile = 0;
		sim->coders[i].previous_compile = -1;
		if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
			return (1);
		sim->coders[i].left_d = &sim->dongles[i];
		sim->coders[i].right_d = &sim->dongles[(i + 1) % sim->nb_coders];
		sim->coders[i].first = NULL;
		sim->coders[i].second = NULL;
		sim->coders[i].sim = sim;
		sim->coders[i].thread = 0;
		i++;
	}
	return (0);
}

int	initialize(t_simulation *sim, char **args)
{
	sim->nb_coders = atoi(args[1]);
	sim->burnout_time = atoi(args[2]);
	sim->compile_time = atoi(args[3]);
	sim->debug_time = atoi(args[4]);
	sim->refactor_time = atoi(args[5]);
	sim->nb_compile = atoi(args[6]);
	sim->dongle_cooldown = atoi(args[7]);
	sim->time = 0;
	sim->start_time = 0;
	sim->monitor = 0;
	if (strcmp("fifo", args[8]) == 0)
		sim->scheduler = TRUE;
	else
		sim->scheduler = FALSE;
	if (pthread_mutex_init(&sim->mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->mutex_mon, NULL) != 0)
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
		if (i % 2==0)
		{
			sim->dongles[i].right = &sim->coders[i];
			if (sim->coders[(i + 1) % sim->nb_coders].id != 0)
				sim->dongles[i].left = &sim->coders[i + 1];
		}
		else
		{
			sim->dongles[i].left = &sim->coders[(i + 1) % sim->nb_coders];
			if (sim->coders[(i + 1) % sim->nb_coders].id != 0)
				sim->dongles[i].right = &sim->coders[i];
		}
		i++;
	}
	return (0);
}
