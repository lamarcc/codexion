/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 22:04:12 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/09 05:42:28 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	is_simulation_over(t_simulation *sim)
{
	pthread_mutex_lock(&sim->mutex);
	if (sim->end_simulation)
	{
		pthread_mutex_unlock(&sim->mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->mutex);
	return (0);
}

int	has_coder_finished(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	if (coder->nb_compile == coder->sim->nb_compile)
	{
		coder->finished = TRUE;
		pthread_mutex_unlock(&coder->mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}
