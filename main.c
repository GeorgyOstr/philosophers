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

void	*philosopher_routine(void *arg)
{
	t_philo_info	*philosopher;
	int				*action;

	action = malloc(4);
	philosopher = arg;
	think_routine(philosopher);
	eat_routine(philosopher);
	sleep_routine(philosopher);
	return (action);
}

void	*monitor_routine(void *arg)
{
	return (NULL);
}

int	overflow(int i, int n)
{
	if (i < 0)
		return (n - 1);
	return (i);
}

void	inititialize_mutexes(pthread_mutex_t *mutexes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_init(mutexes + i, NULL);
		i++;
	}
}

void inititialize_philosophers(t_philo_info *philosophers, pthread_mutex_t *forks,
		int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		philosophers[i].thread_num = i + 1;
		if (i % 2 == 0)
		{
			philosophers[i].right_fork = forks + overflow(i - 1, n);
			philosophers[i].left_fork = forks + i;
		}
		else
		{
			philosophers[i].left_fork = forks + overflow(i - 1, n);
			philosophers[i].right_fork = forks + i;
		}
		i++;
	}
}


void	start_simulation(t_arguments args)
{
	int				i;
	t_philo_info	*philosophers;
	pthread_mutex_t	*forks;

	philosophers = calloc(args.number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	forks = calloc(args.number_of_philosophers, sizeof(*forks));
	if (forks == NULL)
		error_exit(MALLOC_ERROR);
	inititialize_mutexes(forks, args.number_of_philosophers);
	inititialize_philosophers(philosophers, forks, args.number_of_philosophers);
	i = 0;
	while (i < args.number_of_philosophers)
	{
		if (pthread_create(&(philosophers[i].thread_id), NULL,
				&philosopher_routine, &(philosophers[i])) != 0)
			error_exit(THREAD_ERROR);
		i++;
	}
	i = 0;
	while (i < args.number_of_philosophers)
	{
		if (pthread_join(philosophers[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
		i++;
	}
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
		// printf("%i, %i, %i, %i, %i\n", args.number_of_philosophers,
		//	args.time_to_die, args.time_to_eat, args.time_to_sleep,
		//	args.number_of_eat_to_finish);
	}
	else
	{
		error_exit(ARGUMENTS_ERROR);
	}
	start_simulation(args);
	return (0);
}
