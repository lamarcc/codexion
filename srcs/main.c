/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:05:50 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/17 22:26:13 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	waiting_start(t_coder *coder)
{
	int	check;

	check = 0;
	while (1)
	{
		pthread_mutex_lock(&coder->sim->mutex);
		check = coder->sim->coders_can_start;
		pthread_mutex_unlock(&coder->sim->mutex);
		if (check != 0)
			return (check);
		usleep(1000);
	}
	return (0);
}

static int	start_simulation(t_simulation *sim)
{
	int	i;

	start_time(sim);
	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				&coder_routine, &sim->coders[i]) != 0)
			return (set_start(sim, -1));
		sim->coder_thread_success += 1;
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, &monitor_routine, sim) != 0)
		return (set_start(sim, -1));
	set_start(sim, TRUE);
	sim->monitor_thread_success += 1;
	return (0);
}

int	main(int c, char **v)
{
	t_simulation	*sim;

	sim = ft_calloc(1, sizeof(t_simulation));
	if (!sim)
		return (error(sim));
	if (c != 9)
		return (error(sim));
	if (check_args(sim, v))
		return (error(sim));
	if (initialize(sim, v))
	{
		cleanup(sim);
		fprintf(stderr, "Error on initialization\n");
		return (1);
	}
	start_simulation(sim);
	join_thread(sim);
	cleanup(sim);
	return (0);
}
