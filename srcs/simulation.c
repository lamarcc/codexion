/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/28 05:41:28 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	take_dongle(t_coder *coder)
{
	t_dongle	*min;
	t_dongle	*max;

	pthread_mutex_lock(&coder->mutex);
	if (coder->left_d->id < coder->right_d->id)
	{
		max = coder->right_d;
		min = coder->left_d;
	}
	else
	{
		max = coder->left_d;
		min = coder->right_d;
	}
	pthread_mutex_unlock(&coder->mutex);
	if (!min->taken)
	{
		if (!max->taken)
		{
			min->taken = 1;
			max->taken = 1;
			return (0);
		}
		return (1);
	}
	return (1);
}

int	leave_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->left_d->taken = 0;
	coder->right_d->taken = 0;
	pthread_mutex_unlock(&coder->mutex);
	return (0);
}

void	*routine(void *arg)
{
	t_coder		*coder;
	pthread_t	t;
	// bool	f;

	coder = (t_coder *)arg;
	t = pthread_self();
	printf("coder left: %d, coder right: %d\n", coder->left_d->first->id, coder->right_d->first->id);
	// pthread_mutex_lock(&coder->mutex);
	// if (coder->left_d->first == coder && coder->right_d->first == coder)
	// 	f = true;
	// else
	// 	f = false;
	// pthread_mutex_unlock(&coder->mutex);
	// if (f)
	// {
	// 	if (!take_dongle(coder))
	// 	{
	// 		pthread_mutex_lock(&coder->sim->mutex_log);
	// 		printf("%d, %ld take dongles\n", coder->id, t);
	// 		pthread_mutex_unlock(&coder->sim->mutex_log);
	// 		leave_dongle(coder);
	// 	}
	// }
	// else
	// {
	// 	pthread_mutex_lock(&coder->sim->mutex_log);
	// 	printf("no dongle for : %d, %ld\n", coder->id, t);
	// 	pthread_mutex_unlock(&coder->sim->mutex_log);
	// }
	return (NULL);
}

int	run(t_simulation *sim)
{
	int	i;

	int a = 0;
	while (a < 2)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_create(&sim->coders[i].thread, NULL, &routine, &sim->coders[i]);
			usleep(10);
			i++;
		}
		// printf("\n");
		// i = 0;
		// while (i < sim->nb_coders)
		// {
		// 	printf("dongles n%d, first coder: %d, second coder: %d\n", i, sim->dongles[i].first->id, sim->dongles[i].second->id);
		// 	i++;
		// }
		// printf("\nAfter swap\n");
		// i = 0;
		// t_coder *t;
		// while (i < sim->nb_coders)
		// {
		// 	t = sim->dongles[i].first;
		// 	sim->dongles[i].first = sim->dongles[i].second;
		// 	sim->dongles[i].second = t;
		// 	printf("dongles n%d, first coder: %d, second coder: %d\n", i, sim->dongles[i].first->id, sim->dongles[i].second->id);
		// 	i++;
		// }
		a++;
	}
	return (1);
}
