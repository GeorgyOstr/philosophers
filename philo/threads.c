/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 19:49:16 by gostroum          #+#    #+#             */
/*   Updated: 2026/03/06 19:49:18 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	create_philo_threads(t_sim_info *sim)
{
	int	i;
	int	err;

	i = 0;
	sim->threads_created = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		err = pthread_create(&sim->philos[i].thread_id, NULL, &philo_routine,
				&sim->philos[i]);
		if (err != 0)
		{
			pthread_mutex_lock(sim->philos[i].mutexes->finish);
			*sim->philos[i].is_simulation_finished = 1;
			pthread_mutex_unlock(sim->philos[i].mutexes->finish);
			return (THREAD_ERROR);
		}
		sim->threads_created = i + 1;
		i++;
	}
	return (0);
}

int	join_threads(t_sim_info *sim)
{
	int	i;
	int	err;
	int	ret;

	i = 0;
	ret = 0;
	while (i < sim->threads_created)
	{
		err = pthread_join(sim->philos[i].thread_id, NULL);
		if (err != 0)
			ret = THREAD_ERROR_1;
		i++;
	}
	return (ret);
}
