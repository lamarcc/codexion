/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:47:29 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/20 05:10:25 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	*routine(void *arg)
{
	t_coder		*coder;
	pthread_t	t;
	bool		res;

	coder = (t_coder *)arg;
	t = pthread_self();
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->sim->mutex_sim);
		coder->sim->bite = true;
		pthread_mutex_unlock(&coder->sim->mutex_sim);
	}
	else
	{
		pthread_mutex_lock(&coder->sim->mutex_sim);
		res = coder->sim->bite;
		pthread_mutex_unlock(&coder->sim->mutex_sim);
		pthread_mutex_lock(&coder->sim->mutex_log);
		printf("%ld,  |||  bool = %d\n", t, res);
		pthread_mutex_unlock(&coder->sim->mutex_log);
	}
	return (NULL);
}

int	run(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL, &routine, &sim->coders[i]);
		usleep(10);
		i++;
	}
	return (1);
}
