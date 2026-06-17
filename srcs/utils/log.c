/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 02:29:34 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/18 00:44:44 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	print_log(t_coder *coder, char *str)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d %s\n", get_time(coder->sim), coder->id, str);
	pthread_mutex_unlock(&coder->sim->mutex);
}
