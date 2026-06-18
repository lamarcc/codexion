/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celamarc <celamarc@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 00:45:55 by celamarc          #+#    #+#             */
/*   Updated: 2026/06/18 05:06:40 by celamarc         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define TRUE 1
# define FALSE 0
# define ARG_EXEMPLE "Follow: ./codexion <number_of_coders> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<number_of_compiles_required> <dongle_cooldown> <scheduler>"

typedef struct s_simulation
{
	int					error_check;
	int					nb_coders;
	int					nb_compile;
	int					end_simulation;
	int					scheduler;
	int					coders_can_start;
	int					coder_thread_success;
	int					monitor_thread_success;
	int					sim_mutex_init;
	int					coders_mutex_init;
	int					dongles_mutex_init;
	int					dongles_cond_init;
	long				burnout_time;
	long				compile_time;
	long				debug_time;
	long				refactor_time;
	long				dongle_cooldown;
	long				start_time;
	char				*errors;
	pthread_t			monitor;
	pthread_mutex_t		mutex;
	struct s_coder		*coders;
	struct s_dongle		*dongles;
}		t_simulation;

typedef struct s_coder
{
	int				id;
	int				nb_compile;
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
	pthread_cond_t	cond;
	pthread_mutex_t	mutex;
	struct s_coder	*queue[2];
}		t_dongle;

void	compile_log(t_coder *coder);
void	debug_log(t_coder *coder);
void	refactor_log(t_coder *coder);
void	enter_queue(t_coder *coder, t_dongle *dongle);
void	leave_queue(t_dongle *dongle);
void	leave_dongle(t_coder *coder);
void	update_compile_time(t_coder *coder);
void	start_time(t_simulation *sim);
void	cleanup(t_simulation *sim);
void	join_thread(t_simulation *sim);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	*ft_calloc(size_t len, size_t size);
long	time_since(t_simulation *sim, long time);
long	get_time(t_simulation *sim);
int		smart_sleep(int ms, t_simulation *sim);
int		set_start(t_simulation *sim, int state);
int		d_cooldown(t_simulation *sim, t_dongle *dongle);
int		initialize(t_simulation *sim, char **args);
int		check_args(t_simulation	*sim, char **args);
int		error(t_simulation *sim);
int		is_simulation_over(t_simulation *sim);
int		take_dongle(t_coder *coder);
int		waiting_start(t_coder *coder);
int		scheduler(t_coder *coder, t_dongle *dongle);
int		has_coder_finished(t_coder *coder);
int		ft_atoi(char *str);

#endif
