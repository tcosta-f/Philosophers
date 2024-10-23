/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:22:23 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 01:02:02 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void		ft_start_monitor(t_table *table);
static int	ft_check_philo_status(t_philo *philo);
void		ft_handle_error(char *str, t_table *table, int error_stage);
static void	ft_cleanup(t_table *table, int error_stage);
static void	ft_destroy_mutexes(t_table *table, int error_stage);

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
	return (0);
}

void	ft_handle_error(char *str, t_table *table, int error_stage)
{
	if (str && *str)
		while (*str)
			write(2, str++, 1);
	if (error_stage)
		ft_cleanup(table, error_stage);
	exit(1);
}

static void	ft_cleanup(t_table *table, int error_stage)
{
	int	i;

	i = -1;
	if (error_stage >= 1 && table->philo)
	{
		while (++i < table->n_philo)
		{
			if (error_stage >= 3)
			{
				if (pthread_join(table->philo[i].thread, NULL))
					ft_handle_error("Error: Failed to join thread", table, 2);
			}
		}
		free(table->philo);
	}
	if (error_stage >= 2)
		ft_destroy_mutexes(table, error_stage);
}

static void	ft_destroy_mutexes(t_table *table, int error_stage)
{
	int	i;

	i = 0;
	if (error_stage >= 2 && table->forks)
	{
		while (i < table->n_philo)
		{
			pthread_mutex_destroy(&table->forks[i]);
			i++;
		}
		free(table->forks);
	}
	if (error_stage >= 1)
	{
		pthread_mutex_destroy(&table->start_mtx);
		pthread_mutex_destroy(&table->print_mtx);
		pthread_mutex_destroy(&table->eat_mtx);
		pthread_mutex_destroy(&table->end_mtx);
	}
}
