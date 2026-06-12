/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:24:18 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/11 22:17:26 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	enter_queue(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	else if (dongle->queue[1] == NULL)
		dongle->queue[1] = coder;
	pthread_mutex_unlock(&dongle->mutex);
}

void	leave_queue(t_dongle *dongle)
{
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = NULL;
}
