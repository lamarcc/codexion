/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/30 05:09:43 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	update_time(t_simulation *sim)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	pthread_mutex_lock(&sim->mutex_sim);
	sim->time = (t.tv_sec * 1000) + (t.tv_usec / 1000) - sim->start_time;
	pthread_mutex_unlock(&sim->mutex_sim);
}

void	*routine(void *arg)
{
	t_coder			*coder;

	coder = (t_coder *)arg;
	while (TRUE)
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
		pthread_mutex_lock(&coder->first->mutex);
		if (coder->first->waiting[0] == NULL)
			coder->first->waiting[0] = coder;
		else
			coder->first->waiting[1] = coder;
		while (coder->first->waiting[0] != coder || coder->first->taken)
			pthread_cond_wait(&coder->first->cond, &coder->first->mutex);
		coder->first->taken = TRUE;
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_lock(&coder->second->mutex);
		if (coder->second->waiting[0] == NULL)
			coder->second->waiting[0] = coder;
		else
			coder->second->waiting[1] = coder;
		while (coder->second->waiting[0] != coder || coder->second->taken)
			pthread_cond_wait(&coder->second->cond, &coder->second->mutex);
		coder->second->taken = TRUE;
		pthread_mutex_unlock(&coder->second->mutex);
		update_time(coder->sim);
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
		printf("%ld %d has taken a dongle\n", coder->sim->time, coder->id);
		printf("%ld %d is compiling\n", coder->sim->time, coder->id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
		usleep(coder->sim->compile_time * 1000);
		pthread_mutex_lock(&coder->first->mutex);
		coder->first->taken = FALSE;
		coder->first->waiting[0] = coder->first->waiting[1];
		coder->first->waiting[1] = NULL;
		pthread_cond_broadcast(&coder->first->cond);
		pthread_mutex_unlock(&coder->first->mutex);
		pthread_mutex_lock(&coder->second->mutex);
		coder->second->taken = FALSE;
		coder->second->waiting[0] = coder->second->waiting[1];
		coder->second->waiting[1] = NULL;
		pthread_cond_broadcast(&coder->second->cond);
		pthread_mutex_unlock(&coder->second->mutex);
		update_time(coder->sim);
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is debugging\n", coder->sim->time, coder->id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
		usleep(coder->sim->debug_time * 1000);
		update_time(coder->sim);
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld %d is refactoring\n", coder->sim->time, coder->id);
		pthread_mutex_unlock(&coder->sim->mutex_log);
		usleep(coder->sim->refactor_time * 1000);
		pthread_mutex_lock(&coder->sim->mutex_sim);
		coder->nb_compile += 1;
		if (coder->nb_compile == 10)
		{
			pthread_mutex_unlock(&coder->sim->mutex_sim);
			break ;
		}
		pthread_mutex_unlock(&coder->sim->mutex_sim);
	}
	return (NULL);
}

int	run(t_simulation *sim)
{
	int	i;
	struct timeval t;

	int a = 0;
	gettimeofday(&t, NULL);
	pthread_mutex_lock(&sim->mutex_sim);
	sim->start_time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	pthread_mutex_unlock(&sim->mutex_sim);
	while (a < 1)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_create(&sim->coders[i].thread, NULL, &routine, &sim->coders[i]);
			// regler le retour
			usleep(10);
			i++;
		}
		// while (i < sim->nb_coders)
		// {
		// 	printf("dongles n%d, left coder: %d, right coder: %d\n", i, sim->dongles[i].left->id, sim->dongles[i].right->id);
		// 	i++;
		// }
		// printf("\nAfter swap\n");
		// i = 0;
		// t_coder *t;
		// while (i < sim->nb_coders)
		// {
		// 	t = sim->dongles[i].right;
		// 	sim->dongles[i].right = sim->dongles[i].left;
		// 	sim->dongles[i].left = t;
		// 	printf("dongles n%d, left coder: %d, right coder: %d\n", i, sim->dongles[i].left->id, sim->dongles[i].right->id);
		// 	i++;
		// }
		a++;
	}
	return (1);
}
