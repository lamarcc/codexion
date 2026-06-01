/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:00:19 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/01 04:54:37 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	find_first(t_coder *coder)
{
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
}

int	take_dongle(t_coder *coder)
{
	struct timeval	t;
	long			time;

	find_first(coder);
	pthread_mutex_lock(&coder->first->mutex);
	if (coder->first->queue[0] == NULL)
		coder->first->queue[0] = coder;
	else
		coder->first->queue[1] = coder;
	while (coder->first->queue[0] != coder || coder->first->taken)
	{
		pthread_cond_wait(&coder->first->cond, &coder->first->mutex);
		while (1)
		{
			gettimeofday(&t, NULL);
			time = ((t.tv_sec * 1000) + (t.tv_usec / 1000) - coder->sim->start_time);
			// if (coder->nb_compile == 1)
			// {
			// 	printf("%ld     %ld\n", coder->first->last_released + coder->sim->dongle_cooldown, time);
			// 	exit(1);
			// }
			if (time > coder->first->last_released + coder->sim->dongle_cooldown)
				break ;
		}
	}
	if (coder->sim->end_simulation)
	{
		pthread_mutex_unlock(&coder->first->mutex);
		return (1);
	}
	pthread_mutex_lock(&coder->second->mutex);
	if (coder->second->queue[0] == NULL)
		coder->second->queue[0] = coder;
	else
		coder->second->queue[1] = coder;
	while (coder->second->queue[0] != coder || coder->second->taken)
	{
		pthread_cond_wait(&coder->second->cond, &coder->second->mutex);
		while (1)
		{
			gettimeofday(&t, NULL);
			time = ((t.tv_sec * 1000) + (t.tv_usec / 1000) - coder->sim->start_time);
			if (time > coder->second->last_released + coder->sim->dongle_cooldown)
				break ;
		}
	}
	coder->first->taken = TRUE;
	coder->second->taken = TRUE;
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_unlock(&coder->second->mutex);
	return (0);
}

void	leave_dongle(t_coder *coder)
{
	struct timeval	t;
	long			time;

	pthread_mutex_lock(&coder->first->mutex);
	coder->first->taken = FALSE;
	coder->first->queue[0] = coder->first->queue[1];
	coder->first->queue[1] = NULL;
	gettimeofday(&t, NULL);
	time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	coder->first->last_released = time - coder->sim->start_time;
	pthread_cond_broadcast(&coder->first->cond);
	pthread_mutex_unlock(&coder->first->mutex);
	pthread_mutex_lock(&coder->second->mutex);
	coder->second->taken = FALSE;
	coder->second->queue[0] = coder->second->queue[1];
	coder->second->queue[1] = NULL;
	gettimeofday(&t, NULL);
	time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	coder->second->last_released = time - coder->sim->start_time;
	pthread_cond_broadcast(&coder->second->cond);
	pthread_mutex_unlock(&coder->second->mutex);
}
