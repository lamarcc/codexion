/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 02:29:34 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/07 02:19:10 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	compile_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), id);
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), id);
		printf("%ld %d is compiling\n", get_time(coder->sim), id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	debug_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is debugging\n", get_time(coder->sim), id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	refactor_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is refactoring\n", get_time(coder->sim), id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	burn_log(t_simulation *sim, int id)
{
	pthread_mutex_lock(&sim->mutex_log);
	printf("%ld %d burned out\n", get_time(sim), id);
	pthread_mutex_unlock(&sim->mutex_log);
}
