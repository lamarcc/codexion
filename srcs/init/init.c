/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 03:42:28 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 02:59:01 by celamarc         ###   ########lyon.fr   */
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
		sim->dongles[i].last_released = -sim->dongle_cooldown;
		sim->dongles[i].queue[0] = NULL;
		sim->dongles[i].queue[1] = NULL;
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
		sim->coders[i].previous_compile = -1;
		sim->coders[i].left_d = &sim->dongles[i];
		sim->coders[i].right_d = &sim->dongles[(i - 1 + sim->nb_coders)
			% sim->nb_coders];
		sim->coders[i].first = NULL;
		sim->coders[i].second = NULL;
		sim->coders[i].thread = 0;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	initialize(t_simulation *sim, char **args)
{
	sim->nb_coders = ft_atoi(args[1]);
	sim->burnout_time = ft_atoi(args[2]);
	sim->compile_time = ft_atoi(args[3]);
	sim->debug_time = ft_atoi(args[4]);
	sim->refactor_time = ft_atoi(args[5]);
	sim->nb_compile = ft_atoi(args[6]);
	sim->dongle_cooldown = ft_atoi(args[7]);
	sim->start_time = 0;
	sim->monitor = 0;
	if (strcmp("fifo", args[8]) == 0)
		sim->scheduler = TRUE;
	else
		sim->scheduler = FALSE;
	if (pthread_mutex_init(&sim->mutex, NULL) != 0)
		return (1);
	sim->coders = calloc(sim->nb_coders, sizeof(t_coder));
	sim->dongles = calloc(sim->nb_coders, sizeof(t_dongle));
	if (init_dongles(sim))
		return (1);
	if (init_coders(sim))
		return (1);
	return (0);
}
