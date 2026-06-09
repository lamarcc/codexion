/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:19 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/09 05:43:49 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

static int	d_cooldown(t_simulation *sim, t_dongle *d1, t_dongle *d2, long c)
{
	return (time_since(sim, d1->last_released + c) >= 0
		&& time_since(sim, d2->last_released + c) >= 0);
}

static int	find_first_dongle(t_coder *coder)
{
	if (coder->left_d == coder->right_d)
		return (1);
	if (coder->left_d->id > coder->right_d->id)
	{
		coder->first = coder->right_d;
		coder->second = coder->left_d;
	}
	else
	{
		coder->first = coder->left_d;
		coder->second = coder->right_d;
	}
	return (0);
}

int	take_dongle(t_coder *coder)
{
	if (find_first_dongle(coder))
		return (1);
	pthread_mutex_lock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	enter_queue(coder, coder->first);
	enter_queue(coder, coder->second);
	if (coder->sim->scheduler)
	{
		if ((!coder->first->taken && !coder->second->taken) && d_cooldown(coder->sim, coder->first, coder->second, coder->sim->dongle_cooldown))
		{
			coder->first->taken = true;
			coder->second->taken = true;
			pthread_mutex_unlock(&coder->first->mutex);
			pthread_mutex_unlock(&coder->second->mutex);
			return (0);
		}
	}
	else
	{
		if ((!coder->first->taken && !coder->second->taken) && d_cooldown(coder->sim, coder->first, coder->second, coder->sim->dongle_cooldown) && coder->first->queue[0] == coder && coder->second->queue[0] == coder)
		{
			coder->first->taken = true;
			coder->second->taken = true;
			pthread_mutex_unlock(&coder->first->mutex);
			pthread_mutex_unlock(&coder->second->mutex);
			return (0);
		}
	}
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_unlock(&coder->second->mutex);
	return (1);
}

void	leave_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	coder->first->taken = FALSE;
	coder->second->taken = FALSE;
	leave_queue(coder->first);
	leave_queue(coder->second);
	coder->first->last_released = get_time(coder->sim);
	coder->second->last_released = get_time(coder->sim);
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_unlock(&coder->second->mutex);
}
