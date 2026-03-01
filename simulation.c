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

void	inititialize_philosophers(t_philo_info *philosophers,
		pthread_mutex_t *forks, int n)
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

void	create_threads(t_philo_info *philosophers, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_create(&(philosophers[i].thread_id), NULL,
				&philosopher_routine, &(philosophers[i])) != 0)
			error_exit(THREAD_ERROR);
		i++;
	}
}

void	join_threads(t_philo_info *philosophers, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_join(philosophers[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
		i++;
	}
}
