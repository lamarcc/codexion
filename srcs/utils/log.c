/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 02:29:34 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/10 05:45:25 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

// UNIFIER LES PRINT DANS UNE FONCTIN ET ENVOYER LA STR DIRECTEMENT
// ATOI DOIT RENVOOYER 0 SI LETTRE
// SCHEDULER EN MAJ DOIT ETRE ACCEPTER

void	print_coder(t_coder *coder, int id, char *actions)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d %s\n", get_time(coder->sim), id, actions);
	pthread_mutex_unlock(&coder->sim->mutex);

}

void	compile_log(t_coder *coder, int id)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
	{
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), id);
		printf("%ld %d has taken a dongle\n", get_time(coder->sim), id);
		printf("%ld %d is compiling\n", get_time(coder->sim), id);
	}
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	debug_log(t_coder *coder, int id)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d is debugging\n", get_time(coder->sim), id);
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	refactor_log(t_coder *coder, int id)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (!coder->sim->end_simulation)
		printf("%ld %d is refactoring\n", get_time(coder->sim), id);
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	burn_log(t_simulation *sim, int id)
{
	pthread_mutex_lock(&sim->mutex);
	printf("%ld %d burned out\n", get_time(sim), id);
	pthread_mutex_unlock(&sim->mutex);
}
