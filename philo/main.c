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

static int	parse_args(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
		return (ARGUMENTS_ERROR);
	if (parse_positive_int(argv[1], &args->number_of_philos))
		return (ATOI_ERROR);
	if (parse_positive_int(argv[2], &args->time_to_die))
		return (ATOI_ERROR);
	if (parse_positive_int(argv[3], &args->time_to_eat))
		return (ATOI_ERROR);
	if (parse_positive_int(argv[4], &args->time_to_sleep))
		return (ATOI_ERROR);
	args->number_of_eat_to_finish = -1;
	if (argc == 6)
	{
		if (parse_positive_int(argv[5], &args->number_of_eat_to_finish))
			return (ATOI_ERROR);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_args		args;
	t_sim_info	sim;
	int			err;

	err = parse_args(argc, argv, &args);
	if (err)
		return (report_error(err));
	ft_bzero(&sim, sizeof(sim));
	err = populate_info(&sim, &args);
	if (err)
	{
		clean_up(&sim);
		return (report_error(err));
	}
	err = start_simulation(&sim);
	clean_up(&sim);
	if (err)
		return (report_error(err));
	return (0);
}

int	allocate_all(t_sim_info *sim, t_args *args)
{
	sim->philos = ft_calloc(args->number_of_philos, sizeof(*sim->philos));
	if (sim->philos == NULL)
		return (MALLOC_ERROR);
	sim->forks = ft_calloc(args->number_of_philos, sizeof(*sim->forks));
	if (sim->forks == NULL)
		return (MALLOC_ERROR);
	sim->forks_states = ft_calloc(args->number_of_philos,
			sizeof(*sim->forks_states));
	if (sim->forks_states == NULL)
		return (MALLOC_ERROR);
	return (0);
}

void	populate_philo(t_sim_info *sim, t_args *args, int i)
{
	sim->philos[i].args = args;
	sim->philos[i].philo_num = i + 1;
	sim->philos[i].is_simulation_finished = &sim->is_simulation_finished;
	sim->philos[i].sim_start = &sim->sim_start;
	sim->philos[i].eat_enough_count = &sim->eat_enough_count;
	sim->philos[i].last_ate_time = 0;
	sim->philos[i].eat_count = 0;
	sim->forks_states[i] = 0;
}

int	populate_info(t_sim_info *sim, t_args *args)
{
	int	i;
	int	err;

	sim->is_simulation_finished = 0;
	sim->eat_enough_count = 0;
	err = allocate_all(sim, args);
	if (err)
		return (err);
	i = 0;
	while (i < args->number_of_philos)
	{
		populate_philo(sim, args, i);
		i++;
	}
	err = initialize_mutexes(sim);
	if (err)
		return (err);
	initialize_philos(sim);
	sim->sim_start = get_time();
	return (0);
}
