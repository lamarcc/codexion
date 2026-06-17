/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 05:42:32 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/16 23:26:34 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	error(t_simulation *sim)
{
	if (!sim)
	{
		fprintf(stderr, "Error on initialization\n");
		return (1);
	}
	if (sim->errors)
		printf("\n%s\n", sim->errors);
	printf("Follow this exemple: %s\n", ARG_EXEMPLE);
	if (sim->errors)
		free(sim->errors);
	free(sim);
	return (1);
}

void	join_thread(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_thread_success)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (sim->monitor_thread_success)
		pthread_join(sim->monitor, NULL);
}

static void	cleanup_coders_and_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->dongles_mutex_init)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	i = 0;
	while (i < sim->dongles_cond_init)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	i = 0;
	while (i < sim->coders_mutex_init)
	{
		pthread_mutex_destroy(&sim->coders[i].mutex);
		i++;
	}
}

void	cleanup(t_simulation *sim)
{
	cleanup_coders_and_dongles(sim);
	if (sim->dongles)
		free(sim->dongles);
	if (sim->coders)
		free(sim->coders);
	if (sim->sim_mutex_init)
		pthread_mutex_destroy(&sim->mutex);
	free(sim);
}
