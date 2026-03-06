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

void	create_threads(t_philo_info *philosophers)
{
	int	i;

	i = 0;
	while (i < philosophers[0].args->number_of_philosophers)
	{
		if (pthread_create(&(philosophers[i].thread_id), NULL,
				&philosopher_routine, &(philosophers[i])) != 0)
			error_exit(THREAD_ERROR);
		i++;
	}
}

void	join_threads(t_philo_info *philosophers, t_monitor_info *monitor_info)
{
	int	i;

	i = 0;
	if (pthread_join(monitor_info->thread_id, NULL) != 0)
		error_exit(THREAD_ERROR_1);
	while (i < philosophers[0].args->number_of_philosophers)
	{
		if (pthread_join(philosophers[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
		i++;
	}
}
