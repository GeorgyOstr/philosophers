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
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>

# define ARGUMENTS_ERROR	1
# define ATOI_ERROR			2
# define MALLOC_ERROR		3
# define THREAD_ERROR		4
# define THREAD_ERROR_1		5

typedef struct s_arguments
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_eat_to_finish;
}	t_arguments;

typedef struct s_philo_info
{
	pthread_t		thread_id;
	int				thread_num;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philo_info;

void	error_exit(int err_num);
size_t	ft_strlen(char *str);
long	ft_atoi(char *str);

void	eat_routine(t_philo_info *philosopher);
void	sleep_routine(t_philo_info *philosopher);
void	think_routine(t_philo_info *philosopher);

void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
void	start_simulation(t_arguments args);


#endif
