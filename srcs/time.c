/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 23:03:54 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/31 01:42:06 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	start_time(t_simulation *sim)
{
	pthread_mutex_lock(&sim->mutex);
	gettimeofday(&sim->t, NULL);
	sim->start_time = (sim->t.tv_sec * 1000) + (sim->t.tv_usec / 1000);
	pthread_mutex_unlock(&sim->mutex);
}

void	update_time(t_simulation *sim)
{
	long	sec;
	long	usec;

	gettimeofday(&sim->t, NULL);
	sec = sim->t.tv_sec * 1000;
	usec = sim->t.tv_usec / 1000;
	sim->time = (sec + usec) - sim->start_time;
}

void	update_compile_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->previous_compile = coder->sim->time;
	coder->sim->coder_verif = coder;
	pthread_mutex_unlock(&coder->mutex);
}
