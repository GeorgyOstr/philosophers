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
	think_routine(philosopher);
	eat_routine(philosopher);
	sleep_routine(philosopher);
	return (arg);
}

void	*monitor_routine(void *arg)
{
	t_all	*args_and_philosophers;

	args_and_philosophers = arg;
	while (1)
	{
		pthread_mutex_lock(args_and_philosophers->args->finished_eating);
		if (min_eat_amount(args_and_philosophers->philosophers) == args_and_philosophers->args->number_of_eat_to_finish)
		{
			printf("%ld All philosophers have eaten at least %d times.\n",
				get_time(),
				args_and_philosophers->args->number_of_eat_to_finish);
			exit(0);
		}
		pthread_mutex_unlock(args_and_philosophers->args->finished_eating);
	}
	return (arg);
}

void	initialize_monitor(t_arguments *args, t_philo_info *philosophers)
{
	pthread_t	monitor_thread;
	t_all		all;

	all.args = args;
	all.philosophers = philosophers;
	pthread_create(&monitor_thread, NULL, monitor_routine, &all);
	pthread_detach(monitor_thread);
}

void	start_simulation(t_arguments *args)
{
	int				i;
	t_philo_info	*philosophers;
	pthread_mutex_t	*forks;

	philosophers = calloc(args->number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	forks = calloc(args->number_of_philosophers, sizeof(*forks));
	if (forks == NULL)
		error_exit(MALLOC_ERROR);
	initialize_mutexes(forks, args);
	initialize_philosophers(philosophers, forks, args);
	initialize_monitor(args, philosophers);
	create_threads(philosophers);
	join_threads(philosophers);
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
