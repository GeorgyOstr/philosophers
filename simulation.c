/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 01:06:16 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/02 01:06:19 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	start_simulation(t_arguments *args)
{
	t_philo_info	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	finish_mutex;
	t_monitor_info	monitor_info;

	philosophers = calloc(args->number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	forks = calloc(args->number_of_philosophers, sizeof(*forks));
	if (forks == NULL)
		error_exit(MALLOC_ERROR);
	args->meal_mutex = &meal_mutex;
	args->write_mutex = &write_mutex;
	args->finish_mutex = &finish_mutex;
	initialize_mutexes(forks, args);
	initialize_philosophers(philosophers, forks, args);
	initialize_monitor(&monitor_info, args, philosophers);
	create_threads(philosophers);
	join_threads(philosophers, &monitor_info);
	freeall(philosophers, forks, args);
}