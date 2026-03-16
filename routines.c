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

void	*philo_routine(void *arg)
{
	t_philo_info	*philo;

	philo = arg;
	if (philo->last_ate_time == 0)
		philo->last_ate_time = get_time();
	while (1)
	{
		pthread_mutex_lock(philo->mutexes->finish);
		if (*philo->is_simulation_finished)
		{
			pthread_mutex_unlock(philo->mutexes->finish);
			break ;
		}
		pthread_mutex_unlock(philo->mutexes->finish);
		if (think_routine(philo))
			break ;
		if (eat_routine(philo))
			break ;
		if (sleep_routine(philo))
			break ;
	}
	return (arg);
}

int	think_routine(t_philo_info *philo)
{
	if (print_status(philo, THINKING))
		return (1);
	if (philo->philo_num % 2 == 0 && philo->eat_count == 0)
		usleep(philo->args->time_to_eat * 1000 / 2);
	else if (philo->eat_count > 1 && philo->args->number_of_philos % 2 == 1)
		if (busy_sleep(philo, philo->args->time_to_eat / 2))
			return (1);
	while (grabbing_fork(philo, 0))
		if (check_dead(philo))
			return (1);
	if (print_status(philo, TAKEN_FORK))
		return (release_fork(philo, 0), 1);
	while (grabbing_fork(philo, 1))
		if (check_dead(philo))
			return (release_fork(philo, 0), 1);
	if (print_status(philo, TAKEN_FORK))
		return (release_fork(philo, 0), release_fork(philo, 1), 1);
	return (0);
}

int	eat_routine(t_philo_info *philo)
{
	if (print_status(philo, EATING))
		return (release_fork(philo, 0), release_fork(philo, 1), 1);
	philo->eat_count++;
	philo->last_ate_time = get_time();
	if (philo->eat_count == philo->args->number_of_eat_to_finish)
	{
		pthread_mutex_lock(philo->mutexes->finish);
		*philo->eat_enough_count += 1;
		if (*philo->eat_enough_count == philo->args->number_of_philos
			&& !*philo->is_simulation_finished)
		{
			*philo->is_simulation_finished = 1;
			pthread_mutex_lock(philo->mutexes->write);
			printf("All philos have eaten at least %d times.\n",
				philo->args->number_of_eat_to_finish);
			pthread_mutex_unlock(philo->mutexes->write);
		}
		pthread_mutex_unlock(philo->mutexes->finish);
	}
	if (busy_sleep(philo, philo->args->time_to_eat))
		return (release_fork(philo, 0), release_fork(philo, 1), 1);
	return (release_fork(philo, 0), release_fork(philo, 1), 0);
}

int	sleep_routine(t_philo_info *philo)
{
	if (print_status(philo, SLEEPING))
		return (1);
	if (busy_sleep(philo, philo->args->time_to_sleep))
		return (1);
	return (0);
}
