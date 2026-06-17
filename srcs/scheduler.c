/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 03:38:31 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/18 00:02:23 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	fifo_loop(t_coder *coder, t_dongle *dongle)
{
	while (dongle->taken || !d_cooldown(coder->sim, dongle))
	{
		if (dongle->taken)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		while (1)
		{
			if (d_cooldown(coder->sim, dongle))
				return (1);
			if (smart_sleep(1, coder->sim))
				return (0);
		}
	}
	if (is_simulation_over(coder->sim))
		return (0);
	return (1);
}

static int	edf_loop(t_coder *coder, t_dongle *dongle)
{
	while ((dongle->queue[0] != coder || dongle->taken
			|| !d_cooldown(coder->sim, dongle)))
	{
		if (dongle->queue[0] != coder)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		while (1)
		{
			if (d_cooldown(coder->sim, dongle))
				return (1);
			if (smart_sleep(1, coder->sim))
				return (0);
		}
	}
	if (is_simulation_over(coder->sim))
		return (0);
	return (1);
}

int	scheduler(t_coder *coder, t_dongle *dongle)
{
	if (is_simulation_over(coder->sim))
		return (0);
	if (coder->sim->scheduler)
		if (fifo_loop(coder, dongle))
			return (1);
	if (!coder->sim->scheduler)
		if (edf_loop(coder, dongle))
			return (1);
	return (0);
}
