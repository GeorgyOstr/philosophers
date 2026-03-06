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
	while (get_time()
		- philosopher->last_ate_time < philosopher->args->time_to_die)
	{
		think_routine(philosopher);
		eat_routine(philosopher);
		sleep_routine(philosopher);
	}
	return (arg);
}

void	*monitor_routine(void *arg)
{
	t_monitor_info	*monitor_info;

	monitor_info = arg;
	while (1)
	{
		pthread_mutex_lock(monitor_info->args->finished_eating);
		if (min_eat_amount(monitor_info->philosophers) == monitor_info->args->number_of_eat_to_finish)
		{
			pthread_mutex_lock(monitor_info->args->write_mutex);
			printf("%ld All philosophers have eaten at least %d times.\n",
				get_time(),
				monitor_info->args->number_of_eat_to_finish);
			exit(0);
		}
		pthread_mutex_unlock(monitor_info->args->finished_eating);
	}
	return (arg);
}

void	initialize_monitor(t_monitor_info *monitor_info, t_arguments *args, t_philo_info *philosophers)
{
	monitor_info->args = args;
	monitor_info->philosophers = philosophers;
	pthread_create(&monitor_info->thread_id, NULL, monitor_routine, monitor_info);
}

void	start_simulation(t_arguments *args)
{
	t_philo_info	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	finished_eating;
	pthread_mutex_t	write_mutex;
	t_monitor_info	monitor_info;

	philosophers = calloc(args->number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	forks = calloc(args->number_of_philosophers, sizeof(*forks));
	if (forks == NULL)
		error_exit(MALLOC_ERROR);
	args->finished_eating = &finished_eating;
	args->write_mutex = &write_mutex;
	initialize_mutexes(forks, args);
	initialize_philosophers(philosophers, forks, args);
	initialize_monitor(&monitor_info, args, philosophers);
	create_threads(philosophers);
	join_threads(philosophers, &monitor_info);
	destroy_mutexes(forks, args);
	free(philosophers);
	free(forks);
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
