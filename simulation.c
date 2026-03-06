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

int	overflow(int i, int n)
{
	if (i < 0)
		return (n - 1);
	return (i);
}

void	initialize_mutexes(pthread_mutex_t *mutexes, t_arguments *args)
{
	int	i;

	pthread_mutex_init(args->finished_eating, NULL);
	pthread_mutex_init(args->write_mutex, NULL);
	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_init(mutexes + i, NULL);
		i++;
	}
}

void	initialize_philosophers(t_philo_info *philosophers,
		pthread_mutex_t *forks, t_arguments *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		philosophers[i].args = args;
		philosophers[i].eat_count = 0;
		philosophers[i].last_ate_time = 0;
		{
			philosophers[i].thread_num = i + 1;
			if (i % 2 == 0)
			{
				philosophers[i].right_fork = forks + overflow(i - 1,
						args->number_of_philosophers);
				philosophers[i].left_fork = forks + i;
			}
			else
			{
				philosophers[i].left_fork = forks + overflow(i - 1,
						args->number_of_philosophers);
				philosophers[i].right_fork = forks + i;
			}
			i++;
		}
	}
}

void	create_threads(t_philo_info *philosophers)
{
	int	i;

	i = 0;
	while (i < philosophers[0].args->number_of_philosophers)
	{
		if (pthread_create(&(philosophers[i].thread_id), NULL,
				&philosopher_routine, &(philosophers[i])) != 0)
			error_exit(THREAD_ERROR);
		i++;
	}
}

void	join_threads(t_philo_info *philosophers, t_monitor_info *monitor_info)
{
	int	i;

	i = 0;
	if(pthread_join(monitor_info->thread_id, NULL) != 0)
		error_exit(THREAD_ERROR_1);
	while (i < philosophers[0].args->number_of_philosophers)
	{
		if (pthread_join(philosophers[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
		i++;
	}
}
