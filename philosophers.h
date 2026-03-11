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
	pthread_mutex_t	*meal;
	pthread_mutex_t	*write;
	pthread_mutex_t	*finish;
}					t_mutexes;

typedef struct s_philo_info
{
	pthread_t		thread_id;
	pthread_mutex_t	*forks[2];
	t_args			*args;
	t_mutexes		*mutexes;
	int				*is_someone_died;
	int				*is_simulation_finished;
	long			last_ate_time;
	int				philo_num;
	int				eat_count;
	int				is_dead;
	int				*sim_start;
}					t_philo_info;

typedef struct s_sim_info
{
	pthread_t		thread_id;
	t_philo_info	*philos;
	pthread_mutex_t	*forks;
	t_mutexes		mutexes;
	pthread_mutex_t	meal;
	pthread_mutex_t	write;
	pthread_mutex_t	finish;
	int				is_someone_died;
	int				is_simulation_finished;
	int				sim_start;
}					t_sim_info;

void				populate_info(t_sim_info *sim, t_args *args);
void				error_exit(int err_num);

void				initialize_mutexes(t_sim_info *sim);
void				initialize_philos(t_sim_info *sim);
void				clean_up(t_sim_info *sim);

void				start_simulation(t_sim_info *sim);
int					check_dead(t_philo_info *philo);
int					check_anyone_dead(t_sim_info *sim);	
long				get_time(void);

void				create_philo_threads(t_sim_info *philos);
void				create_monitor_thread(t_sim_info *sim);
void				join_threads(t_sim_info *sim);

void				*philo_routine(void *arg);
void				think_routine(t_philo_info *philo);
void				eat_routine(t_philo_info *philo);
void				sleep_routine(t_philo_info *philo);
void				*monitor_routine(void *arg);

int					overflow(int i, int n);
int					min_eat_amount(t_philo_info *philos);
long				ft_atoi(char *str);

void				print_status(t_philo_info *philo, int status);
#endif
