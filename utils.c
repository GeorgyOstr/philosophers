/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:21:27 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 15:49:14 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	overflow(int i, int n)
{
	if (i < 0)
		return (n - 1);
	return (i);
}

int	min_eat_amount(t_philo_info *philos)
{
	int	i;
	int	min;

	min = philos[0].eat_count;
	i = 0;
	while (i < philos[0].args->number_of_philos)
	{
		if (philos[i].eat_count < min)
			min = philos[i].eat_count;
		i++;
	}
	return (min);
}

long	ft_atoi(char *str)
{
	int			i;
	long		ans;
	const int	len = ft_strlen(str);

	i = 0;
	ans = 0;
	if (len == 20 || len == 0)
		error_exit(ATOI_ERROR);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			error_exit(ATOI_ERROR);
		i++;
	}
	while (str[i] && i < 20)
	{
		if ('0' > str[i] || str[i] > '9')
			error_exit(ATOI_ERROR);
		ans = 10 * ans + (str[i] - '0');
		i++;
	}
	if (ans <= 0)
		error_exit(ATOI_ERROR);
	return (ans);
}

int	print_status(t_philo_info *philo, int status)
{
	pthread_mutex_lock(philo->mutexes->finish);
	if (check_dead_already_locked(philo))
	{
		pthread_mutex_unlock(philo->mutexes->finish);
		return (1);
	}
	pthread_mutex_lock(philo->mutexes->write);
	if (status == TAKEN_FORK && !(*philo->is_simulation_finished))
		printf("%ld %d has taken a fork\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == EATING && !(*philo->is_simulation_finished))
		printf("%ld %d is eating\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == SLEEPING && !(*philo->is_simulation_finished))
		printf("%ld %d is sleeping\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == THINKING && !(*philo->is_simulation_finished))
		printf("%ld %d is thinking\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == DIED && philo->is_dead)
		printf("%ld %d died\n", get_time() - *philo->sim_start,
			philo->philo_num);
	else if (status == FINISHED && *philo->is_simulation_finished
		&& !*philo->is_someone_died)
		printf("All philos have eaten at least %d times.\n",
			philo->args->number_of_eat_to_finish);
	pthread_mutex_unlock(philo->mutexes->write);
	pthread_mutex_unlock(philo->mutexes->finish);
	return (0);
}
