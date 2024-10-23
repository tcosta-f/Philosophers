/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:59:30 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 23:19:01 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int			ft_atoi(const char *ptr);
long long	ft_atoll(char *str, int i, long long res);
long long	ft_get_time(void);

int	ft_atoi(const char *ptr)
{
	int	i;
	int	sig;
	int	res;

	i = 0;
	sig = 1;
	res = 0;
	while ((ptr[i] >= 9 && ptr[i] <= 13) || ptr[i] == ' ')
		i++;
	if (ptr[i] == '-' || ptr[i] == '+')
	{
		if (ptr[i] == '-')
			sig = -1;
		i++;
	}
	while (ptr[i] >= '0' && ptr[i] <= '9')
	{
		res *= 10;
		res += (ptr[i] - '0');
		i++;
	}
	return (res * sig);
}

long long	ft_atoll(char *str, int i, long long res)
{
	int	sig;

	sig = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sig = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > (LLONG_MAX / 10)
			|| (res == LLONG_MAX / 10 && (str[i] - '0') > (LLONG_MAX % 10)))
		{
			if (sig == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sig);
}

long long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1e3 + time.tv_usec / 1e3);
}
