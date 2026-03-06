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

void	eat_routine(t_philo_info *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	print_status(philosopher, TAKEN_FORK);
	if (philosopher->args->number_of_philosophers == 1)
	{
		while (get_time()
			- philosopher->last_ate_time < philosopher->args->time_to_die)
			;
		print_status(philosopher, DIED);
	}
	pthread_mutex_lock(philosopher->right_fork);
	print_status(philosopher, TAKEN_FORK);
	print_status(philosopher, EATING);
	philosopher->last_ate_time = get_time();
	while (get_time()
		- philosopher->last_ate_time < philosopher->args->time_to_die
		&& get_time()
		- philosopher->last_ate_time < philosopher->args->time_to_eat)
		;
	if (get_time()
		- philosopher->last_ate_time >= philosopher->args->time_to_die)
		philosopher->args->finish_flag += 1;
	pthread_mutex_lock(philosopher->args->meal_mutex);
	philosopher->eat_count++;
	pthread_mutex_unlock(philosopher->args->meal_mutex);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	sleep_routine(t_philo_info *philosopher)
{
	print_status(philosopher, SLEEPING);
	while (get_time()
		- philosopher->last_ate_time < philosopher->args->time_to_die
		&& get_time()
		- philosopher->last_ate_time < philosopher->args->time_to_sleep)
		;
	if (get_time()
		- philosopher->last_ate_time >= philosopher->args->time_to_die)
	{
		pthread_mutex_lock(philosopher->args->finish_mutex);
		philosopher->args->finish_flag += 1;
		pthread_mutex_unlock(philosopher->args->finish_mutex);
	}
}

void	think_routine(t_philo_info *philosopher)
{
	print_status(philosopher, THINKING);
	if (philosopher->eat_count >= philosopher->args->number_of_eat_to_finish)
	{
		if (get_time()
			- philosopher->last_ate_time >= philosopher->args->time_to_die)
		{
			pthread_mutex_lock(philosopher->args->finish_mutex);
			philosopher->args->finish_flag += 1;
			pthread_mutex_unlock(philosopher->args->finish_mutex);
		}
		else
			usleep((philosopher->args->time_to_die - (get_time()
						- philosopher->last_ate_time)) * 1000
				/ philosopher->args->number_of_philosophers);
	}
}
