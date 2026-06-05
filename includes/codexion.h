/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 00:45:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/05 05:15:09 by celamarc         ###   ########lyon.fr   */
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
	pthread_t			monitor;
	pthread_mutex_t		mutex_sim;
	pthread_mutex_t		mutex_log;
	struct s_coder		*coders;
	struct s_dongle		*dongles;
}		t_simulation;

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	int				has_dongle;
	int				finished;
	long			previous_compile;
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
	struct s_coder	*queue[2];
}		t_dongle;

void	update_queue(t_coder *coder, t_dongle *dongle, int move);
void	compile_log(t_coder *coder, int id);
void	debug_log(t_coder *coder, int id);
void	refactor_log(t_coder *coder, int id);
void	burn_log(t_simulation *sim, int id, long time);
void	cleanup(t_simulation *sim);
void	leave_dongle(t_coder *coder);
void	start_time(t_simulation *sim);
void	update_time(t_simulation *sim);
void	update_compile_time(t_coder *coder);
void	update_dongle_time(long start_time, t_dongle *dongle);
void	*ft_calloc(size_t len, size_t size);
long	get_time(t_simulation *sim);
int		initialize(t_simulation *sim, char **args);
int		check_args(t_simulation	*sim, char **args);
int		run(t_simulation *sim);
int		take_dongle(t_coder *coder);
int		is_simulation_over(t_simulation *sim);
int		has_coder_finished(t_coder *coder);

#endif
