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

long	timestamp_ms(t_arguments *args)
{
	return (get_time() - args->start_time);
}

int	should_stop(t_arguments *args)
{
	int	stop;

	pthread_mutex_lock(args->stop_mutex);
	stop = args->stop;
	pthread_mutex_unlock(args->stop_mutex);
	return (stop);
}

void	set_stop(t_arguments *args, int value)
{
	pthread_mutex_lock(args->stop_mutex);
	args->stop = value;
	pthread_mutex_unlock(args->stop_mutex);
}

void	print_status(t_philo_info *philosopher, char *status)
{
	pthread_mutex_lock(philosopher->args->print_mutex);
	if (!should_stop(philosopher->args))
		printf("%ld %d %s\n", timestamp_ms(philosopher->args),
			philosopher->thread_num, status);
	pthread_mutex_unlock(philosopher->args->print_mutex);
}

void	precise_sleep(long duration_ms, t_arguments *args)
{
	long	start;

	start = get_time();
	while (!should_stop(args) && get_time() - start < duration_ms)
		usleep(200);
}

void	eat_routine(t_philo_info *philosopher)
{
	if (should_stop(philosopher->args))
		return ;
	pthread_mutex_lock(philosopher->left_fork);
	print_status(philosopher, "has taken a fork");
	if (philosopher->args->number_of_philosophers == 1)
	{
		precise_sleep(philosopher->args->time_to_die, philosopher->args);
		set_stop(philosopher->args, 1);
		pthread_mutex_lock(philosopher->args->print_mutex);
		printf("%ld %d died\n", timestamp_ms(philosopher->args),
			philosopher->thread_num);
		pthread_mutex_unlock(philosopher->args->print_mutex);
		pthread_mutex_unlock(philosopher->left_fork);
		return ;
	}
	pthread_mutex_lock(philosopher->right_fork);
	print_status(philosopher, "has taken a fork");
	print_status(philosopher, "is eating");
	pthread_mutex_lock(philosopher->args->finished_eating);
	philosopher->last_ate_time = get_time();
	pthread_mutex_unlock(philosopher->args->finished_eating);
	precise_sleep(philosopher->args->time_to_eat, philosopher->args);
	pthread_mutex_lock(philosopher->args->finished_eating);
	philosopher->eat_count++;
	pthread_mutex_unlock(philosopher->args->finished_eating);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	sleep_routine(t_philo_info *philosopher)
{
	print_status(philosopher, "is sleeping");
	precise_sleep(philosopher->args->time_to_sleep, philosopher->args);
}

void	think_routine(t_philo_info *philosopher)
{
	print_status(philosopher, "is thinking");
}
