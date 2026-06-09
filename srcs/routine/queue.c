/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 23:24:18 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/09 05:43:42 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	enter_queue(t_coder *coder, t_dongle *dongle)
{
	if (dongle->queue[0] == NULL)
		dongle->queue[0] = coder;
	if (dongle->queue[1] != NULL && dongle->queue[1] != coder)
		dongle->queue[1] = coder;
}

void	leave_queue(t_dongle *dongle)
{
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = NULL;
}
