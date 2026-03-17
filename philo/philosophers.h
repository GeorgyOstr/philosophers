/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:16:05 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 18:42:29 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>

# define ARGUMENTS_ERROR 1
# define ATOI_ERROR 2
# define MALLOC_ERROR 3
# define THREAD_ERROR 4
# define THREAD_ERROR_1 5

enum				e_status
{
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	FINISHED
};

typedef struct s_args
{
	int				number_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_eat_to_finish;
}					t_args;

typedef struct s_mutexes
{
	pthread_mutex_t	*write;
	pthread_mutex_t	*finish;
}					t_mutexes;

typedef struct s_philo_info
{
	pthread_t		thread_id;
	pthread_mutex_t	*forks[2];
	int				*forks_states[2];
	t_args			*args;
	t_mutexes		*mutexes;
	int				*is_simulation_finished;
	int				*eat_enough_count;
	long			*sim_start;
	long			last_ate_time;
	int				philo_num;
	int				eat_count;
}					t_philo_info;

typedef struct s_sim_info
{
	t_philo_info	*philos;
	pthread_mutex_t	*forks;
	int				*forks_states;
	t_mutexes		mutexes;
	pthread_mutex_t	write;
	pthread_mutex_t	finish;
	int				is_simulation_finished;
	long			sim_start;
	int				eat_enough_count;
	int				threads_created;
	int				forks_mutex_count;
	int				write_initialized;
	int				finish_initialized;
}					t_sim_info;

int					allocate_all(t_sim_info *sim, t_args *args);
int					populate_info(t_sim_info *sim, t_args *args);
int					report_error(int err_num);

int					initialize_mutexes(t_sim_info *sim);
void				initialize_philos(t_sim_info *sim);
void				clean_up(t_sim_info *sim);

int					start_simulation(t_sim_info *sim);
int					check_dead(t_philo_info *philo);
int					check_dead_already_locked(t_philo_info *philo);
long				get_time(void);
int					print_status(t_philo_info *philo, int status);

int					create_philo_threads(t_sim_info *philos);
int					join_threads(t_sim_info *sim);

int					busy_sleep(t_philo_info *philo, int duration);
int					grabbing_fork(t_philo_info *philo, int num);
int					release_fork(t_philo_info *philo, int num);

void				*philo_routine(void *arg);
int					think_routine(t_philo_info *philo);
int					eat_routine(t_philo_info *philo);
int					sleep_routine(t_philo_info *philo);

int					overflow(int i, int n);
int					parse_positive_int(char *str, int *out);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t count, size_t size);

#endif
