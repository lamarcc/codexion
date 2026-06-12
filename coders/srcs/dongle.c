/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:19 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/11 22:06:35 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

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
	if (!coder->sim->scheduler)
	{
		enter_queue(coder, coder->first);
		enter_queue(coder, coder->second);
	}
	return (0);
}

int	take_dongle(t_coder *coder)
{
	if (find_first_dongle(coder))
		return (1);
	pthread_mutex_lock(&coder->first->mutex);
	if (!scheduler(coder, coder->first))
	{
		pthread_mutex_unlock(&coder->first->mutex);
		return (1);
	}
	coder->first->taken = TRUE;
	pthread_mutex_unlock(&coder->first->mutex);
	if (is_simulation_over(coder->sim))
		return (1);
	pthread_mutex_lock(&coder->second->mutex);
	if (!scheduler(coder, coder->second))
	{
		pthread_mutex_unlock(&coder->second->mutex);
		return (1);
	}
	coder->second->taken = TRUE;
	pthread_mutex_unlock(&coder->second->mutex);
	return (0);
}

void	leave_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->first->mutex);
	coder->first->taken = FALSE;
	coder->first->last_released = get_time(coder->sim);
	if (!coder->sim->scheduler)
		leave_queue(coder->first);
	pthread_cond_broadcast(&coder->first->cond);
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	coder->second->taken = FALSE;
	coder->second->last_released = get_time(coder->sim);
	if (!coder->sim->scheduler)
		leave_queue(coder->second);
	pthread_cond_broadcast(&coder->second->cond);
	pthread_mutex_unlock(&coder->second->mutex);
}
