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

void	create_philo_threads(t_sim_info *sim)
{
	int	i;

	i = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL, &philo_routine,
				&sim->philos[i]) != 0)
			error_exit(THREAD_ERROR);
		i++;
	}
}

void	create_monitor_thread(t_sim_info *sim)
{
	pthread_create(&sim->thread_id, NULL, &monitor_routine, sim);
}

void	join_threads(t_sim_info *sim)
{
	int	i;

	i = 0;
	if (pthread_join(sim->thread_id, NULL) != 0)
		error_exit(THREAD_ERROR_1);
	while (i < sim->philos->args->number_of_philos)
	{
		if (pthread_join(sim->philos[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
		i++;
	}
}
