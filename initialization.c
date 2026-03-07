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


void	initialize_mutexes(t_sim_info *sim)
{
	int	i;

	pthread_mutex_init(sim->philos->mutexes->meal, NULL);
	pthread_mutex_init(sim->philos->mutexes->write, NULL);
	pthread_mutex_init(sim->philos->mutexes->finish, NULL);
	
	i = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		pthread_mutex_init(sim->forks + i, NULL);
		i++;
	}
}

void	initialize_philos(t_sim_info *sim)
{
	int	i;

	i = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		{
			if (i % 2 == 0)
			{
				sim->philos[i].forks[0] = sim->forks + overflow(i - 1,
						sim->philos->args->number_of_philos);
				sim->philos[i].forks[1] = sim->forks + i;
			}
			else
			{
				sim->philos[i].forks[1] = sim->forks + overflow(i - 1,
						sim->philos->args->number_of_philos);
				sim->philos[i].forks[0] = sim->forks + i;
			}
			i++;
		}
	}
}

void	clean_up(t_sim_info *sim)
{
	int	i;

	pthread_mutex_destroy(sim->philos->mutexes->meal);
	pthread_mutex_destroy(sim->philos->mutexes->write);
	pthread_mutex_destroy(sim->philos->mutexes->finish);
	i = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		pthread_mutex_destroy(sim->forks + i);
		i++;
	}
	free(sim->forks);
	free(sim->philos);
}
