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

void	start_simulation(t_sim_info *sim)
{
	create_philo_threads(sim);
	join_threads(sim);
	clean_up(sim);
}

int	check_dead(t_philo_info *philo)
{
	pthread_mutex_lock(philo->mutexes->finish);
	if (get_time() - philo->last_ate_time >= philo->args->time_to_die 
		|| *philo->is_simulation_finished)
	{
		if (!*philo->is_simulation_finished)
		{
			*philo->is_simulation_finished = 1;
			pthread_mutex_lock(philo->mutexes->write);
			printf("%ld %d died\n", get_time() - *philo->sim_start, philo->philo_num);
			pthread_mutex_unlock(philo->mutexes->write);
		}
		pthread_mutex_unlock(philo->mutexes->finish);
		return (1);
	}
	pthread_mutex_unlock(philo->mutexes->finish);
	return (0);
}
int	check_dead_already_locked(t_philo_info *philo)
{
	if (get_time() - philo->last_ate_time >= philo->args->time_to_die 
		|| *philo->is_simulation_finished)
	{
		if (!*philo->is_simulation_finished)
		{
			*philo->is_simulation_finished = 1;
			pthread_mutex_lock(philo->mutexes->write);
			printf("%ld %d died\n", get_time() - *philo->sim_start, philo->philo_num);
			pthread_mutex_unlock(philo->mutexes->write);
		}
		return (1);
	}
	return (0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
