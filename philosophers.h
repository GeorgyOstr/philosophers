/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostroum <gostroum@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 15:16:05 by gostroum          #+#    #+#             */
/*   Updated: 2026/02/24 18:42:29 by gostroum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define ARGUMENTS_ERROR	1
#define ATOI_ERROR		2
#define MALLOC_ERROR	3
#define THREAD_ERROR	4
#define THREAD_ERROR_1	5

typedef struct s_arguments
{
		int number_of_philosophers;
		int time_to_die;
		int	time_to_eat;
		int	time_to_sleep;
		int number_of_eat_to_finish;
} t_arguments;

void	error_exit(int err_num);
size_t	ft_strlen(char *str);
long	ft_atoi(char *str);
#endif
