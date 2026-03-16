/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minift.c                                           :+:      :+:    :+:   */
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

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*ptr;

	ptr = s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total_size;

	if (size != 0 && count > INT_MAX / size)
		return (NULL);
	total_size = count * size;
	ptr = malloc(total_size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}

int	overflow(int i, int n)
{
	if (i < 0)
		return (n - 1);
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
