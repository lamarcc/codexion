/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:05:50 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/11 05:23:50 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	error(t_simulation *sim)
{
	if (!sim)
	{
		printf("Error on initialization\n");
		return (1);
	}
	if (sim->errors)
		printf("\n%s\n", sim->errors);
	printf("Follow this exemple: %s\n", ARG_EXEMPLE);
	free(sim->errors);
	free(sim);
	return (1);
}

static void	join_thread(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}

static void	cleanup(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	pthread_mutex_destroy(&sim->mutex);
	free(sim);
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
			return (1);
		usleep(100);
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, &monitor_routine, sim) != 0)
		return (1);
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
		return (1);
	}
	start_simulation(sim);
	join_thread(sim);
	cleanup(sim);
	return (0);
}
