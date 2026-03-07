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
	pthread_mutex_lock(philo->mutexes->finish);
	while (!(*philo->is_simulation_finished))
	{
		pthread_mutex_unlock(philo->mutexes->finish);
		think_routine(philo);
		eat_routine(philo);
		sleep_routine(philo);
		pthread_mutex_lock(philo->mutexes->finish);
	}
	pthread_mutex_unlock(philo->mutexes->finish);
	if (philo->is_dead)
		print_status(philo, DIED);
	return (arg);
}

void	think_routine(t_philo_info *philo)
{
	print_status(philo, THINKING);
	pthread_mutex_lock(philo->mutexes->meal);
	if (philo->eat_count >= philo->args->number_of_eat_to_finish)
	{
		pthread_mutex_unlock(philo->mutexes->meal);
		if (!check_dead(philo))
		{
			usleep((philo->args->time_to_die - (get_time()
						- philo->last_ate_time)) * 1000
				/ (philo->args->number_of_philos + 1));
		}
		pthread_mutex_lock(philo->mutexes->meal);
	}
	pthread_mutex_unlock(philo->mutexes->meal);
}

void	eat_routine(t_philo_info *philo)
{
	pthread_mutex_lock(philo->forks[0]);
	print_status(philo, TAKEN_FORK);
	if (philo->args->number_of_philos == 1)
	{
		while (get_time() - philo->last_ate_time < philo->args->time_to_die)
			;
		pthread_mutex_unlock(philo->forks[0]);
		check_dead(philo);
		return ;
	}
	pthread_mutex_lock(philo->forks[1]);
	print_status(philo, TAKEN_FORK);
	print_status(philo, EATING);
	philo->last_ate_time = get_time();
	while (get_time() - philo->last_ate_time < philo->args->time_to_die
		&& get_time() - philo->last_ate_time < philo->args->time_to_eat)
		;
	check_dead(philo);
	pthread_mutex_lock(philo->mutexes->meal);
	philo->eat_count++;
	pthread_mutex_unlock(philo->mutexes->meal);
	pthread_mutex_unlock(philo->forks[0]);
	pthread_mutex_unlock(philo->forks[1]);
}

void	sleep_routine(t_philo_info *philo)
{
	print_status(philo, SLEEPING);
	while (get_time() - philo->last_ate_time < philo->args->time_to_die
		&& get_time() - philo->last_ate_time < philo->args->time_to_sleep)
		;
	check_dead(philo);
}

void	*monitor_routine(void *arg)
{
	t_sim_info	*sim;

	sim = arg;
	pthread_mutex_lock(sim->philos->mutexes->finish);
	while (!sim->is_simulation_finished)
	{
		pthread_mutex_unlock(sim->philos->mutexes->finish);
		pthread_mutex_lock(sim->philos->mutexes->meal);
		if (min_eat_amount(sim->philos)
			== sim->philos->args->number_of_eat_to_finish)
		{
			pthread_mutex_lock(sim->philos->mutexes->finish);
			sim->is_simulation_finished = 1;
			pthread_mutex_unlock(sim->philos->mutexes->finish);
		}
		pthread_mutex_unlock(sim->philos->mutexes->meal);
		pthread_mutex_lock(sim->philos->mutexes->finish);
	}
	pthread_mutex_unlock(sim->philos->mutexes->finish);
	print_status(sim->philos, FINISHED);
	return (arg);
}
