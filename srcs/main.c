/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:05:50 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/19 05:34:56 by celamarc         ###   ########lyon.fr   */
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

int	cleanup(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].mutex);
		free(sim->dongles[i].heap);
		i++;
	}
	free(sim->dongles);
	free(sim->coders);
	pthread_mutex_destroy(&sim->mutex_log);
	pthread_mutex_destroy(&sim->mutex_sim);
	free(sim);
	return (0);
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
		return (error(sim));
	printf("%d\n", sim->nb_coders);
	printf("%ld\n", sim->time_burnout);
	printf("%ld\n", sim->time_compile);
	printf("%ld\n", sim->time_debug);
	printf("%ld\n", sim->time_refactor);
	printf("%d\n", sim->nb_compile);
	printf("%ld\n", sim->dongle_cooldown);
	printf("%d\n", sim->scheduler);
	cleanup(sim);
}
