/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 00:31:59 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/02 00:32:00 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void death_exit(t_philo_info *philosopher)
{
	printf("%ld %d died\n", get_time(), philosopher->thread_num);
	exit(0);
}

void	eat_routine(t_philo_info *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	printf("%ld %d has taken a fork\n", get_time(), philosopher->thread_num);
	pthread_mutex_lock(philosopher->right_fork);
	printf("%ld %d has taken a fork\n", get_time(), philosopher->thread_num);
	printf("%ld %d is eating\n", get_time(), philosopher->thread_num);
	philosopher->last_ate_time = get_time();
	while (get_time() - philosopher->last_ate_time < philosopher->args->time_to_die 
			&& get_time() - philosopher->last_ate_time < philosopher->args->time_to_eat)
		;
	if (get_time() - philosopher->last_ate_time >= philosopher->args->time_to_die)
		death_exit(philosopher);
	pthread_mutex_lock(philosopher->args->finished_eating);
	philosopher->eat_count++;
	pthread_mutex_unlock(philosopher->args->finished_eating);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	sleep_routine(t_philo_info *philosopher)
{
	printf("%ld %d is sleeping\n", get_time(), philosopher->thread_num);
	while (get_time() - philosopher->last_ate_time < philosopher->args->time_to_die 
			&& get_time() - philosopher->last_ate_time < philosopher->args->time_to_sleep)
		;
}

void	think_routine(t_philo_info *philosopher)
{
	printf("%ld %d is thinking\n", get_time(), philosopher->thread_num);
}
