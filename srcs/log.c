/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 02:29:34 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/04 23:21:08 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	compile_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d has taken a dongle\n", coder->sim->time, id);
		printf("%ld %d has taken a dongle\n", coder->sim->time, id);
		printf("%ld %d is compiling\n", coder->sim->time, id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	debug_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is debugging\n", coder->sim->time, id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	refactor_log(t_coder *coder, int id)
{
	if (!is_simulation_over(coder->sim))
	{
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is refactoring\n", coder->sim->time, id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
}

void	burn_log(t_simulation *sim, int id, long time)
{
	pthread_mutex_lock(&sim->mutex_log);
	printf("%ld %d burned out\n", time, id);
	pthread_mutex_unlock(&sim->mutex_log);
}
