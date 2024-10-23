/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:16:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 23:17:21 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void		ft_handle_error(char *str, t_table *table, int error_stage);
static void	ft_cleanup(t_table *table, int error_stage);
static void	ft_destroy_mutexes(t_table *table, int error_stage);

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
