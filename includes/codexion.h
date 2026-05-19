/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 00:45:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/19 05:21:49 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

# define TRUE 1
# define FALSE 0
# define ARG_EXEMPLE "Follow: ./codexion <number_of_coders> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<number_of_compiles_required> <dongle_cooldown> <scheduler>"

typedef struct s_simulation
{
	int					nb_coders;
	int					nb_compile;
	int					end_simulation;
	int					scheduler;
	long				time_burnout;
	long				time_compile;
	long				time_debug;
	long				time_refactor;
	long				dongle_cooldown;
	long				start_time;
	char				*errors;
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

void	*ft_calloc(size_t len, size_t size);
int		initialize(t_simulation *sim, char **args);
int		check_args(t_simulation	*sim, char **args);

#endif
