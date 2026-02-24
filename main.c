/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:16:18 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 18:42:54 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arg)
{
	write(1, "hi\n", 3);
	usleep(1000000);
}



typedef struct s_philo_info 
{
		pthread_t	thread_id;
		int			thread_num;
} t_philo_info;

void *philosopher_routine(void *arg)
{
		t_philo_info *philosopher;
		int	*action;
		action = malloc(4);
		philosopher = arg;
		printf("Thread %d: Started philosopher\n", philosopher->thread_num);

		action[0] = 1;
		return action;
}

void start_simulation(t_arguments args)
{
	int i;
	
	t_philo_info *philosophers;
	philosophers = calloc(args.number_of_philosophers, sizeof(*philosophers));
	if (philosophers == NULL)
		error_exit(MALLOC_ERROR);
	i = 0;
	while (i < args.number_of_philosophers)
	{
			philosophers[i].thread_num = i + 1;
			if (pthread_create(&(philosophers[i].thread_id), NULL, &philosopher_routine, &(philosophers[i])) != 0)
					error_exit(THREAD_ERROR);
			i++;
	}
	i = 0;
	while (i < args.number_of_philosophers)
	{
		if (pthread_join(philosophers[i].thread_id, NULL) != 0)
			error_exit(THREAD_ERROR_1);
			
	}
	free(philosophers);
}

int main(int argc, char **argv)
{
	t_arguments args = {};
	if (argc == 5 || argc == 6)
	{
		args.number_of_philosophers = ft_atoi(argv[1]);
		args.time_to_die = ft_atoi(argv[2]);
		args.time_to_eat = ft_atoi(argv[3]);
		args.time_to_sleep = ft_atoi(argv[4]);
		args.number_of_eat_to_finish = -1;
		if (argc == 6)
			args.number_of_eat_to_finish = ft_atoi(argv[5]);
		//printf("%i, %i, %i, %i, %i\n", args.number_of_philosophers, args.time_to_die, args.time_to_eat, args.time_to_sleep, args.number_of_eat_to_finish);

	}
	else
	{
			error_exit(ARGUMENTS_ERROR);
	}
	start_simulation(args);
	return (0);
}
