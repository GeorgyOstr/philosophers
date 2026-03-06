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

typedef struct s_arguments
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_eat_to_finish;
	int				finish_flag;
	pthread_mutex_t	*meal_mutex;
	pthread_mutex_t	*write_mutex;
	pthread_mutex_t	*finish_mutex;
}					t_arguments;

typedef struct s_philo_info
{
	pthread_t		thread_id;
	int				thread_num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_ate_time;
	int				eat_count;
	t_arguments		*args;
}					t_philo_info;
typedef struct s_monitor_info
{
	pthread_t		thread_id;
	t_arguments		*args;
	t_philo_info	*philosophers;
}					t_monitor_info;

void				error_exit(int err_num);
size_t				ft_strlen(char *str);
long				ft_atoi(char *str);
long				get_time(void);

void				eat_routine(t_philo_info *philosopher);
void				sleep_routine(t_philo_info *philosopher);
void				think_routine(t_philo_info *philosopher);

void				*philosopher_routine(void *arg);
void				start_simulation(t_arguments *args);

void				initialize_monitor(t_monitor_info *monitor_info,
						t_arguments *args, t_philo_info *philosophers);
void				*monitor_routine(void *arg);

void				initialize_mutexes(pthread_mutex_t *mutexes,
						t_arguments *args);
void				destroy_mutexes(pthread_mutex_t *mutexes,
						t_arguments *args);
void				initialize_philosophers(t_philo_info *philosophers,
						pthread_mutex_t *forks, t_arguments *args);
void				create_threads(t_philo_info *philosophers);
void				join_threads(t_philo_info *philosophers,
						t_monitor_info *monitor_info);
int					min_eat_amount(t_philo_info *philosophers);

void				print_status(t_philo_info *philosopher, int status);
#endif
