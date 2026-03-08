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
	create_monitor_thread(sim);
	join_threads(sim);
	clean_up(sim);
}

int	check_dead(t_philo_info *philo)
{
	if (get_time() - philo->last_ate_time >= philo->args->time_to_die)
	{
		pthread_mutex_lock(philo->mutexes->finish);
		if (!*philo->is_simulation_finished)
		{
			*philo->is_simulation_finished = 1;
			*philo->is_someone_died = 1;
			philo->is_dead = 1;
		}
		pthread_mutex_unlock(philo->mutexes->finish);
		return (1);
	}
	return (0);
}

int	check_anyone_dead(t_sim_info *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(sim->philos->mutexes->finish);
	while (i < sim->philos->args->number_of_philos)
	{
		if (get_time()
			- sim->philos[i].last_ate_time >= sim->philos[i].args->time_to_die)
		{
			if (!sim->is_simulation_finished)
			{
				sim->philos[i].is_dead = 1;
				sim->is_simulation_finished = 1;
				sim->is_someone_died = 1;
			}
			pthread_mutex_unlock(sim->philos->mutexes->finish);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(sim->philos->mutexes->finish);
	return (0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
