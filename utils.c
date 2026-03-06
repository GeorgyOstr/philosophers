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

void	error_exit(int err_num)
{
	char	c;

	c = '0' + err_num;
	write(1, "Error ", 6);
	write(1, &c, 1);
	write(1, "\n", 1);
	exit(err_num);
}

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] && i < 20)
		i++;
	return (i);
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

void	print_status(t_philo_info *philosopher, int status)
{
	pthread_mutex_lock(philosopher->args->write_mutex);
	pthread_mutex_lock(philosopher->args->finish_mutex);
	if (status == TAKEN_FORK && !philosopher->args->finish_flag)
		printf("%ld %d has taken a fork\n", get_time(), philosopher->thread_num);
	else if (status == EATING && !philosopher->args->finish_flag)
		printf("%ld %d is eating\n", get_time(), philosopher->thread_num);
	else if (status == SLEEPING && !philosopher->args->finish_flag)
		printf("%ld %d is sleeping\n", get_time(), philosopher->thread_num);
	else if (status == THINKING && !philosopher->args->finish_flag)
		printf("%ld %d is thinking\n", get_time(), philosopher->thread_num);
	else if (status == DIED && !philosopher->args->finish_flag == 1)
		printf("%ld %d died\n", get_time(), philosopher->thread_num);
	else if (status == FINISHED && !philosopher->args->finish_flag == 1)
		printf("All philosophers have eaten at least %d times.\n",  philosopher->args->number_of_eat_to_finish);
	pthread_mutex_unlock(philosopher->args->write_mutex);
	pthread_mutex_unlock(philosopher->args->finish_mutex);
}
