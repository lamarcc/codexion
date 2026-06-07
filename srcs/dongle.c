/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:19 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/07 03:19:36 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

// static int	check_dongle(t_coder *coder)
// {
//
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

// static int	cond_loop(t_coder *coder, t_dongle *dongle)
// {
// 	while ((dongle->queue[0] != coder || dongle->taken))
// 	{
// 		if (dongle->queue[0] != coder)
// 			pthread_cond_wait(&dongle->cond, &dongle->mutex);
// 	}
// 	if (is_simulation_over(coder->sim))
// 		return (1);
// 	return (0);
// }

int	take(t_coder *coder)
{
	if (find_first_dongle(coder))
		return (1);
	update_queue(coder, coder->first, 1);
	update_queue(coder, coder->second, 1);
	pthread_mutex_lock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	if (time_since(get_time(coder->sim), coder->first->last_released) && time_since(get_time(coder->sim), coder->first->last_released))
	{
		if (!coder->first->taken && !coder->second->taken)
		{
			if (coder->first->queue[1] && !coder->priority)
			{
				pthread_mutex_unlock(&coder->first->mutex);
				pthread_mutex_unlock(&coder->second->mutex);
				return (1);
			}
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

// int	take_dongle(t_coder *coder)
// {
// 	if (find_first_dongle(coder))
// 		return (1);
// 	update_queue(coder, coder->first, 1);
// 	pthread_mutex_lock(&coder->first->mutex);
// 	if (cond_loop(coder, coder->first))
// 	{
// 		pthread_mutex_unlock(&coder->first->mutex);
// 		return (1);
// 	}
// 	update_queue(coder, coder->second, 1);
// 	pthread_mutex_lock(&coder->second->mutex);
// 	if (cond_loop(coder, coder->second))
// 	{
// 		pthread_mutex_unlock(&coder->first->mutex);
// 		pthread_mutex_unlock(&coder->second->mutex);
// 		return (1);
// 	}
// 	coder->first->taken = TRUE;
// 	coder->second->taken = TRUE;
// 	pthread_mutex_unlock(&coder->first->mutex);
// 	pthread_mutex_unlock(&coder->second->mutex);
// 	return (0);
// }

void	leave_dongle(t_coder *coder)
{
	update_queue(coder, coder->first, 0);
	update_dongle_time(coder->sim->start_time, coder->first);
	update_queue(coder, coder->second, 0);
	update_dongle_time(coder->sim->start_time, coder->second);
	pthread_mutex_lock(&coder->mutex);
	if (coder->priority)
		coder->priority = FALSE;
	pthread_mutex_unlock(&coder->mutex);

}

int	take_dongle(t_coder *coder)
{
	if (find_first_dongle(coder))
		return (1);
	pthread_mutex_lock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	if (!coder->first->taken && !coder->second->taken)
	{
		coder->first->taken = true;
		coder->second->taken = true;
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_unlock(&coder->second->mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_unlock(&coder->second->mutex);
	return (1);
}
