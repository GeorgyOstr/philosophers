/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 01:06:16 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/17 20:44:54 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_simulation(t_sim_info *sim)
{
	int	err;

	err = create_philo_threads(sim);
	if (err)
	{
		join_threads(sim);
		return (err);
	}
	return (join_threads(sim));
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
			printf("%ld %d died\n", get_time() - *philo->sim_start,
				philo->philo_num);
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
			printf("%ld %d died\n", get_time() - *philo->sim_start,
				philo->philo_num);
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

int	print_status(t_philo_info *philo, int status)
{
	pthread_mutex_lock(philo->mutexes->finish);
	if (check_dead_already_locked(philo))
	{
		pthread_mutex_unlock(philo->mutexes->finish);
		return (1);
	}
	pthread_mutex_lock(philo->mutexes->write);
	if (status == TAKEN_FORK && !(*philo->is_simulation_finished))
		printf("%ld %d has taken a fork\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == EATING && !(*philo->is_simulation_finished))
		printf("%ld %d is eating\n", philo->last_ate_time - *philo->sim_start,
			philo->philo_num);
	else if (status == SLEEPING && !(*philo->is_simulation_finished))
		printf("%ld %d is sleeping\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == THINKING && !(*philo->is_simulation_finished))
		printf("%ld %d is thinking\n", get_time() - *philo->sim_start,
			philo->philo_num);
	pthread_mutex_unlock(philo->mutexes->write);
	pthread_mutex_unlock(philo->mutexes->finish);
	return (0);
}
