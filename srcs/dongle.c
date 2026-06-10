/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:19 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 03:09:26 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	verif_queue(t_coder *coder)
{
	return (coder->first->queue[0] == coder
		&& coder->second->queue[0] == coder);
}

long	time_since(t_simulation *sim, long time)
{
	return (get_time(sim) - time);
}

int	d_cooldown(t_simulation *sim, t_dongle *dongle)
{
	return (time_since(sim, dongle->last_released + sim->dongle_cooldown) >= 0);
}

// static int	verif_dongle(t_coder *coder, t_dongle *dongle)
// {
// 	if (dongle->taken)
// 		return (0);
// 	if (!d_cooldown(coder->sim, dongle))
// 		return (0);
// 	return (1);
// }
//
// static int	verif_scheduler(t_coder *coder, t_dongle *dongle)
// {
// 	if (!verif_dongle(coder, dongle))
// 		return (0);
// 	if (!coder->sim->scheduler)
// 		if (!verif_queue(coder))
// 			return (0);
// 	dongle->taken = true;
// 	return (1);
// }

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

static int	cond_loop(t_coder *coder, t_dongle *dongle)
{
	while ((dongle->queue[0] != coder || dongle->taken || !d_cooldown(coder->sim, dongle)))
	{
		if (dongle->queue[0] != coder)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		while (1)
		{
			if (d_cooldown(coder->sim, dongle))
				return (0);
			usleep(1000);
		}
	}
	if (is_simulation_over(coder->sim))
		return (1);
	return (0);
}

int	take_dongle(t_coder *coder)
{
	if (find_first_dongle(coder))
		return (1);
	pthread_mutex_lock(&coder->first->mutex);
	enter_queue(coder, coder->first);
	if (cond_loop(coder, coder->first))
	{
		pthread_mutex_unlock(&coder->first->mutex);
		return (1);
	}
	coder->first->taken = TRUE;
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	enter_queue(coder, coder->second);
	if (cond_loop(coder, coder->second))
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
	leave_queue(coder->first);
	coder->first->last_released = get_time(coder->sim);
	pthread_cond_broadcast(&coder->first->cond);
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	coder->second->taken = FALSE;
	leave_queue(coder->second);
	coder->second->last_released = get_time(coder->sim);
	pthread_cond_broadcast(&coder->second->cond);
	pthread_mutex_unlock(&coder->second->mutex);
}
