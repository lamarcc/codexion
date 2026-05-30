/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 00:45:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/05/30 03:19:01 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

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
	long				burnout_time;
	long				compile_time;
	long				debug_time;
	long				refactor_time;
	long				dongle_cooldown;
	long				start_time;
	long				time;
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
	int				has_dongle;
	long			previous_compile_start;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	struct s_dongle	*left_d;
	struct s_dongle	*right_d;
	struct s_dongle	*first;
	struct s_dongle	*second;
	t_simulation	*sim;
}		t_coder;

typedef struct s_dongle
{
	int				id;
	int				taken;
	long			last_released;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	struct s_coder	*left;
	struct s_coder	*right;
	struct s_coder	*waiting[2];
}		t_dongle;

void	*ft_calloc(size_t len, size_t size);
int		initialize(t_simulation *sim, char **args);
int		check_args(t_simulation	*sim, char **args);
int		run(t_simulation *sim);

#endif
