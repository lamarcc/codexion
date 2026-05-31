/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:05:50 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/31 01:41:46 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	error(t_simulation *sim)
{
	printf("\n%s\n", sim->errors);
	printf("Follow this exemple: %s\n", ARG_EXEMPLE);
	free(sim->errors);
	free(sim);
	return (1);
}

void	cleanup(t_simulation *sim)
{
	int	i;

	i = 0;
	pthread_join(sim->monitor, NULL);
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
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
	pthread_mutex_destroy(&sim->mutex_mon);
	free(sim);
}

int	main(int c, char **v)
{
	t_simulation	*sim;

	sim = ft_calloc(1, sizeof(t_simulation));
	if (c != 9)
		return (error(sim));
	if (check_args(sim, v))
		return (error(sim));
	if (initialize(sim, v))
	{
		cleanup(sim);
		return (1);
	}
	run(sim);
	cleanup(sim);
}
