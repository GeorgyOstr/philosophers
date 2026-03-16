/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 00:31:59 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/02 00:32:00 by gostroum         ###   ########.fr       */
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

void	*philo_routine(void *arg)
{
	t_philo_info	*philo;

	philo = arg;
	if (philo->last_ate_time == 0)
		philo->last_ate_time = get_time();
	pthread_mutex_lock(philo->mutexes->finish);
	while (!*philo->is_simulation_finished)
	{
		pthread_mutex_unlock(philo->mutexes->finish);
		if (think_routine(philo))
			break ;
		if (eat_routine(philo))
			break ;
		if (sleep_routine(philo))
			break ;
		pthread_mutex_lock(philo->mutexes->finish);
	}
	pthread_mutex_unlock(philo->mutexes->finish);
	return (arg);
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

int	think_routine(t_philo_info *philo)
{
	if (print_status(philo, THINKING))
		return (1);
	while (grabbing_fork(philo, 0))
		if (check_dead(philo))
			return (1);
	if (print_status(philo, TAKEN_FORK))
		return (1);
	while (grabbing_fork(philo, 1))
		if (check_dead(philo))
			return (1);
	if (print_status(philo, TAKEN_FORK))
		return (1);
	return (0);
}

int	eat_routine(t_philo_info *philo)
{
	if (print_status(philo, EATING))
		return (1);
	philo->eat_count++;
	philo->last_ate_time = get_time();
	if (philo->eat_count == philo->args->number_of_eat_to_finish)
	{
		pthread_mutex_lock(philo->mutexes->meal);
		*philo->eat_enough_count += 1;
		if (*philo->eat_enough_count == philo->args->number_of_philos)
		{
			pthread_mutex_lock(philo->mutexes->finish);
			if (!*philo->is_simulation_finished)
			{
				*philo->is_simulation_finished = 1;
				pthread_mutex_lock(philo->mutexes->write);
				printf("All philos have eaten at least %d times.\n",
					philo->args->number_of_eat_to_finish);
				pthread_mutex_unlock(philo->mutexes->write);
			}
			pthread_mutex_unlock(philo->mutexes->finish);
			pthread_mutex_unlock(philo->mutexes->meal);
			if (print_status(philo, FINISHED))
				return (1);
			return (1);
		}
		pthread_mutex_unlock(philo->mutexes->meal);
	}
	if (busy_sleep(philo, philo->args->time_to_eat))
		return (1);
	release_fork(philo, 0);
	release_fork(philo, 1);
	return (0);
}

int	sleep_routine(t_philo_info *philo)
{
	if (print_status(philo, SLEEPING))
		return (1);
	if (busy_sleep(philo, philo->args->time_to_sleep))
		return (1);
	return (0);
}
