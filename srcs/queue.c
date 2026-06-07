/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:24:18 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/07 03:17:38 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	swap_queue(t_coder *coder, t_dongle *dongle, int move, int t)
{
	if (move)
	{
		if (dongle->queue[0] == NULL)
			dongle->queue[0] = coder;
		else
			dongle->queue[1] = coder;
	}
	else
	{
		if (coder->previous_compile > t)
		{
			dongle->queue[1] = dongle->queue[0];
			dongle->queue[0] = coder;
		}
		else
			dongle->queue[1] = coder;
	}
}

void	entry(t_coder *coder, t_dongle *dongle)
{
	long	check;

	if (coder->sim->scheduler)
	{
		pthread_mutex_lock(&dongle->mutex);
		swap_queue(coder, dongle, 1, 0);
		pthread_mutex_unlock(&dongle->mutex);
	}
	else
	{
		pthread_mutex_lock(&dongle->mutex);
		if (dongle->queue[0] == NULL)
			dongle->queue[0] = coder;
		else
		{
			pthread_mutex_lock(&dongle->queue[0]->mutex);
			check = dongle->queue[0]->previous_compile;
			pthread_mutex_unlock(&dongle->queue[0]->mutex);
			swap_queue(coder, dongle, 0, check);
		}
		pthread_mutex_unlock(&dongle->mutex);
	}
}

void	leave(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = FALSE;
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = NULL;
	pthread_mutex_unlock(&dongle->mutex);
}

void	update_queue(t_coder *coder, t_dongle *dongle, int move)
{
	if (move)
		entry(coder, dongle);
	else
		leave(dongle);
}
