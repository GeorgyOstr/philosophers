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

int	initialize_mutexes(t_sim_info *sim)
{
	int	i;
	int	err;

	err = pthread_mutex_init(&sim->write, NULL);
	if (err != 0)
		return (THREAD_ERROR);
	sim->write_initialized = 1;
	err = pthread_mutex_init(&sim->finish, NULL);
	if (err != 0)
		return (THREAD_ERROR);
	sim->finish_initialized = 1;
	sim->mutexes.write = &sim->write;
	sim->mutexes.finish = &sim->finish;
	i = 0;
	while (i < sim->philos->args->number_of_philos)
	{
		err = pthread_mutex_init(sim->forks + i, NULL);
		if (err != 0)
			return (THREAD_ERROR);
		sim->forks_mutex_count = i + 1;
		sim->philos[i].mutexes = &sim->mutexes;
		i++;
	}
	return (0);
}

void	initialize_philos(t_sim_info *sim)
{
	static int	i = -1;

	while (++i < sim->philos->args->number_of_philos)
	{
		{
			if (i % 2 == 0)
			{
				sim->philos[i].forks[0] = sim->forks + overflow(i - 1,
						sim->philos->args->number_of_philos);
				sim->philos[i].forks[1] = sim->forks + i;
				sim->philos[i].forks_states[0] = sim->forks_states + overflow(i
						- 1, sim->philos->args->number_of_philos);
				sim->philos[i].forks_states[1] = sim->forks_states + i;
			}
			else
			{
				sim->philos[i].forks[1] = sim->forks + overflow(i - 1,
						sim->philos->args->number_of_philos);
				sim->philos[i].forks[0] = sim->forks + i;
				sim->philos[i].forks_states[1] = sim->forks_states + overflow(i
						- 1, sim->philos->args->number_of_philos);
				sim->philos[i].forks_states[0] = sim->forks_states + i;
			}
		}
	}
}

void	clean_up(t_sim_info *sim)
{
	int	i;

	i = 0;
	while (i < sim->forks_mutex_count)
	{
		pthread_mutex_destroy(sim->forks + i);
		i++;
	}
	if (sim->write_initialized)
		pthread_mutex_destroy(&sim->write);
	if (sim->finish_initialized)
		pthread_mutex_destroy(&sim->finish);
	free(sim->forks);
	free(sim->forks_states);
	free(sim->philos);
}

int	report_error(int err_num)
{
	char	c;

	c = '0' + err_num;
	write(2, "Error ", 6);
	write(2, &c, 1);
	write(2, "\n", 1);
	return (err_num);
}
