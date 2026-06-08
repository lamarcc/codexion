/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:24:18 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/08 01:45:53 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	entry(t_coder *coder, t_dongle *dongle)
{
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
}

void	leave(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = FALSE;
	pthread_mutex_unlock(&dongle->mutex);
}

void	update_queue(t_coder *coder, t_dongle *dongle, int move)
{
	if (move)
		entry(coder, dongle);
	else
		leave(dongle);
}
