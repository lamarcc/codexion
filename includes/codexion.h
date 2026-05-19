/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 00:45:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/19 01:19:39 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

# define TRUE 1
# define FALSE 0

typedef struct s_simulation
{
	int					nb_coders;
	int					compile_required;
	int					end_simulation;
	int					fifo;
	int					edf;
	long				time_burnout;
	long				time_compile;
	long				time_debug;
	long				time_refactor;
	long				dongle_cooldown;
	long				start_time;
	pthread_mutex_t		mutex_log;
	pthread_mutex_t		mutex_sim;
	struct s_coder		*coders;
	struct s_dongle		*dongles;	
}		t_simulation;

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	long			previous_compile_start;
	pthread_mutex_t	mutex;
	struct s_dongle	*left_d;
	struct s_dongle	*right_d;
	t_simulation	*sim;
}		t_coder;

typedef struct s_dongle
{
	int				taken;
	long			last_released;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	struct s_heap	*heap;
	int				heap_size;
}		t_dongle;

typedef struct s_heap
{
	t_coder	*coder;
	long	priority;
}		t_heap;

#endif
