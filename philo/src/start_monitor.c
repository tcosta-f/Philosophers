/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:22:23 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 23:53:36 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void		ft_start_monitor(t_table *table);
static int	ft_check_philo_status(t_philo *philo);
static int	ft_check_philo_full(t_philo *philo);
int			ft_check_end_diner(t_table *table);

void	ft_start_monitor(t_table *table)
{
	int	i;
	int	monitor;

	monitor = 1;
	while (monitor)
	{
		i = 0;
		table->n_philos_full = 0;
		while (i < table->n_philo)
		{
			if (monitor && ft_check_philo_status(&table->philo[i]))
			{
				monitor = 0;
				break ;
			}
			i++;
		}
		usleep(100);
	}
}

static int	ft_check_philo_status(t_philo *philo)
{
	t_table		*table;
	long long	now_time;

	table = philo->table;
	pthread_mutex_lock(&table->eat_mtx);
	now_time = ft_get_time();
	if (now_time - philo->time_last_meal >= table->time_to_die)
	{
		ft_sleep_for_action(philo, 0, "died");
		pthread_mutex_unlock(&table->eat_mtx);
		pthread_mutex_lock(&table->end_mtx);
		table->end_diner = 1;
		pthread_mutex_unlock(&table->end_mtx);
		return (1);
	}
	pthread_mutex_unlock(&table->eat_mtx);
	if (ft_check_philo_full(philo))
		return (1);
	return (0);
}

static int	ft_check_philo_full(t_philo *philo)
{
	t_table	*table;
	int		result;

	table = philo->table;
	result = 0;
	pthread_mutex_lock(&table->eat_mtx);
	if (table->n_meals != -1 && philo->count_meals >= table->n_meals)
	{
		pthread_mutex_unlock(&table->eat_mtx);
		pthread_mutex_lock(&table->end_mtx);
		table->n_philos_full++;
		if (table->n_philos_full >= table->n_philo)
		{
			table->end_diner = 1;
			result = 1;
		}
		pthread_mutex_unlock(&table->end_mtx);
	}
	else
		pthread_mutex_unlock(&table->eat_mtx);
	return (result);
}

int	ft_check_end_diner(t_table *table)
{
	int	ended;

	pthread_mutex_lock(&table->end_mtx);
	ended = table->end_diner;
	pthread_mutex_unlock(&table->end_mtx);
	return (ended);
}
