/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:16:18 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 23:13:14 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_args	args;
	t_sim_info sim;

	if (argc == 5 || argc == 6)
	{
		args.number_of_philos = ft_atoi(argv[1]);
		args.time_to_die = ft_atoi(argv[2]);
		args.time_to_eat = ft_atoi(argv[3]);
		args.time_to_sleep = ft_atoi(argv[4]);
		args.number_of_eat_to_finish = -1;
		if (argc == 6)
			args.number_of_eat_to_finish = ft_atoi(argv[5]);
		populate_info(&sim, &args);
	}
	else
	{
		error_exit(ARGUMENTS_ERROR);
	}
	start_simulation(&sim);
	return (0);
}

void populate_info(t_sim_info *sim, t_args *args)
{
	int	i;

	sim->is_simulation_finished = 0;
	sim->philos = calloc(args->number_of_philos, sizeof(*sim->philos));
	if (sim->philos == NULL)
		error_exit(MALLOC_ERROR);
	sim->forks = calloc(args->number_of_philos, sizeof(*sim->forks));
	if (sim->forks == NULL)
		error_exit(MALLOC_ERROR);
	i = 0;
	while (i < args->number_of_philos)
	{
		sim->philos[i].args = args;
		sim->philos[i].philo_num = i + 1;
		sim->philos[i].is_simulation_finished = &sim->is_simulation_finished;		
		sim->philos[i].eat_count = 0;
		sim->philos[i].last_ate_time = 0;
		sim->philos[i].is_dead = 0;		
		sim->philos[i].ate_enough = 0;
		i++;
	}
	initialize_mutexes(sim);
	initialize_philos(sim);
}

void	error_exit(int err_num)
{
	char	c;

	c = '0' + err_num;
	write(1, "Error ", 6);
	write(1, &c, 1);
	write(1, "\n", 1);
	exit(err_num);
}

