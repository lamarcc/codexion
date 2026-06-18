/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 02:29:34 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/18 05:06:47 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	compile_log(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
	{
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), coder->id);
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), coder->id);
		printf("%ld %d is compiling\n", get_time(coder->sim), coder->id);
	}
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	debug_log(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d is debugging\n", get_time(coder->sim), coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	refactor_log(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d is refactoring\n", get_time(coder->sim), coder->id);
	pthread_mutex_unlock(&coder->sim->mutex);
}
