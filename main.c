/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:16:18 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 23:13:14 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	min_eat_amount(t_philo_info *philosophers)
{
	int	i;
	int	min;

	min = philosophers[0].eat_count;
	i = 0;
	while (i < philosophers[0].args->number_of_philosophers)
	{
		if (philosophers[i].eat_count < min)
			min = philosophers[i].eat_count;
		i++;
	}
	return (min);
}

void	*philosopher_routine(void *arg)
{
	t_philo_info	*philosopher;

	philosopher = arg;
	if (philosopher->last_ate_time == 0)
		philosopher->last_ate_time = get_time();
	pthread_mutex_lock(philosopher->args->finish_mutex);
	while (!philosopher->args->finish_flag)
	{
		pthread_mutex_unlock(philosopher->args->finish_mutex);
		think_routine(philosopher);
		eat_routine(philosopher);
		sleep_routine(philosopher);
		pthread_mutex_lock(philosopher->args->finish_mutex);
	}
	pthread_mutex_unlock(philosopher->args->finish_mutex);
	return (arg);
}

void	*monitor_routine(void *arg)
{
	t_monitor_info	*monitor_info;

	monitor_info = arg;
	pthread_mutex_lock(monitor_info->args->finish_mutex);
	while (!monitor_info->args->finish_flag)
	{
		pthread_mutex_unlock(monitor_info->args->finish_mutex);
		pthread_mutex_lock(monitor_info->args->meal_mutex);
		if (min_eat_amount(monitor_info->philosophers) == monitor_info->args->number_of_eat_to_finish)
		{
			pthread_mutex_lock(monitor_info->args->finish_mutex);
			monitor_info->args->finish_flag += 1;
			pthread_mutex_unlock(monitor_info->args->finish_mutex);
		}
		pthread_mutex_unlock(monitor_info->args->meal_mutex);
	}
	return (arg);
}

void	initialize_monitor(t_monitor_info *monitor_info, t_arguments *args,
		t_philo_info *philosophers)
{
	monitor_info->args = args;
	monitor_info->philosophers = philosophers;
	monitor_info->args->finish_flag = 0;
	pthread_create(&monitor_info->thread_id, NULL, monitor_routine,
		monitor_info);
}

int	main(int argc, char **argv)
{
	t_arguments	args;

	if (argc == 5 || argc == 6)
	{
		args.number_of_philosophers = ft_atoi(argv[1]);
		args.time_to_die = ft_atoi(argv[2]);
		args.time_to_eat = ft_atoi(argv[3]);
		args.time_to_sleep = ft_atoi(argv[4]);
		args.number_of_eat_to_finish = -1;
		if (argc == 6)
			args.number_of_eat_to_finish = ft_atoi(argv[5]);
	}
	else
	{
		error_exit(ARGUMENTS_ERROR);
	}
	start_simulation(&args);
	return (0);
}
