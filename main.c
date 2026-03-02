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

	philosopher = arg;
	think_routine(philosopher);
	eat_routine(philosopher);
	sleep_routine(philosopher);
	return (arg);
}

void	*monitor_routine(void *arg)
{
	return (NULL);
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
	inititialize_mutexes(forks, args->number_of_philosophers);
	inititialize_philosophers(philosophers, forks, args);
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
