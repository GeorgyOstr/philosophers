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
	if (philosopher->thread_num % 2 == 0)
		usleep(500);
	while (!should_stop(philosopher->args))
	{
		eat_routine(philosopher);
		if (should_stop(philosopher->args))
			break ;
		sleep_routine(philosopher);
		if (should_stop(philosopher->args))
			break ;
		think_routine(philosopher);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_all	*args_and_philosophers;
	int		i;

	args_and_philosophers = arg;
	while (!should_stop(args_and_philosophers->args))
	{
		i = 0;
		while (i < args_and_philosophers->args->number_of_philosophers)
		{
			pthread_mutex_lock(args_and_philosophers->args->finished_eating);
			if (get_time() - args_and_philosophers->philosophers[i].last_ate_time
				> args_and_philosophers->args->time_to_die)
			{
				pthread_mutex_unlock(args_and_philosophers->args->finished_eating);
				if (should_stop(args_and_philosophers->args))
					return (NULL);
				set_stop(args_and_philosophers->args, 1);
				pthread_mutex_lock(args_and_philosophers->args->print_mutex);
				printf("%ld %d died\n", timestamp_ms(args_and_philosophers->args),
					args_and_philosophers->philosophers[i].thread_num);
				pthread_mutex_unlock(args_and_philosophers->args->print_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(args_and_philosophers->args->finished_eating);
			i++;
		}
		pthread_mutex_lock(args_and_philosophers->args->finished_eating);
		if (args_and_philosophers->args->number_of_eat_to_finish > 0
			&& min_eat_amount(args_and_philosophers->philosophers)
			>= args_and_philosophers->args->number_of_eat_to_finish)
		{
			pthread_mutex_unlock(args_and_philosophers->args->finished_eating);
			set_stop(args_and_philosophers->args, 1);
			pthread_mutex_lock(args_and_philosophers->args->print_mutex);
			printf("%ld All philosophers have eaten at least %d times.\n",
				timestamp_ms(args_and_philosophers->args),
				args_and_philosophers->args->number_of_eat_to_finish);
			pthread_mutex_unlock(args_and_philosophers->args->print_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(args_and_philosophers->args->finished_eating);
		usleep(1000);
	}
	return (NULL);
}

void	initialize_monitor(t_arguments *args, t_philo_info *philosophers,
	pthread_t *monitor_thread, t_all *all)
{
	all->args = args;
	all->philosophers = philosophers;
	if (pthread_create(monitor_thread, NULL, monitor_routine, all) != 0)
		error_exit(THREAD_ERROR);
}

void	start_simulation(t_arguments *args)
{
	t_philo_info	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	finished_eating;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor_thread;
	t_all			all;

	philosophers = calloc(args->number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	forks = calloc(args->number_of_philosophers, sizeof(*forks));
	if (forks == NULL)
		error_exit(MALLOC_ERROR);
	args->start_time = get_time();
	args->stop = 0;
	args->finished_eating = &finished_eating;
	args->stop_mutex = &stop_mutex;
	args->print_mutex = &print_mutex;
	initialize_mutexes(forks, args);
	initialize_philosophers(philosophers, forks, args);
	initialize_monitor(args, philosophers, &monitor_thread, &all);
	create_threads(philosophers);
	if (pthread_join(monitor_thread, NULL) != 0)
		error_exit(THREAD_ERROR_1);
	join_threads(philosophers);
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
