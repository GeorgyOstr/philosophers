/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 19:49:24 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/06 19:49:26 by gostroum         ###   ########.fr       */
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

	pthread_mutex_init(args->meal_mutex, NULL);
	pthread_mutex_init(args->write_mutex, NULL);
	pthread_mutex_init(args->finish_mutex, NULL);
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

void	freeall(t_philo_info *philosophers, pthread_mutex_t *forks,
		t_arguments *args)
{
	int	i;

	pthread_mutex_destroy(args->meal_mutex);
	pthread_mutex_destroy(args->write_mutex);
	pthread_mutex_destroy(args->finish_mutex);
	i = 0;
	while (i < args->number_of_philosophers)
	{
		pthread_mutex_destroy(forks + i);
		i++;
	}
	free(philosophers);
	free(forks);
}
