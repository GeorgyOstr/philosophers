/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:21:27 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 15:49:14 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	busy_sleep(t_philo_info *philo, int duration)
{
	long	start;

	start = get_time();
	while (get_time() - start < duration)
		if (check_dead(philo))
			return (1);
	return (0);
}

int	grabbing_fork(t_philo_info *philo, int num)
{
	pthread_mutex_lock(philo->forks[num]);
	if (*philo->forks_states[num] == 1)
	{
		pthread_mutex_unlock(philo->forks[num]);
		return (1);
	}
	*philo->forks_states[num] = 1;
	pthread_mutex_unlock(philo->forks[num]);
	return (0);
}

int	release_fork(t_philo_info *philo, int num)
{
	pthread_mutex_lock(philo->forks[num]);
	*philo->forks_states[num] = 0;
	pthread_mutex_unlock(philo->forks[num]);
	return (0);
}
