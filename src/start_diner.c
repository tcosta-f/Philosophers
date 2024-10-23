/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_diner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 19:16:55 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 01:01:35 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void		ft_init_philo_threads(t_table *table);
void		*ft_start_diner(void *data);
static void	ft_diner(t_philo *philo);
static void	ft_handle_sleep(t_philo *philo, t_table *table);
void		ft_sleep_for_action(t_philo *philo, long long va_time_ms,
				char *action);

void	ft_init_philo_threads(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&table->start_mtx);
	while (i < table->n_philo)
	{
		if (pthread_create(&table->philo[i].thread, NULL,
				ft_start_diner, &table->philo[i]))
			ft_handle_error("Error\npthread_create fails\n", table, 2);
		i++;
	}
	pthread_mutex_unlock(&table->start_mtx);
}

void	*ft_start_diner(void *data)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)data;
	table = philo->table;
	pthread_mutex_lock(&table->start_mtx);
	pthread_mutex_unlock(&table->start_mtx);
	if ((!(philo->id % 2)) || (table->n_philo % 2 && table->n_philo > 1
			&& philo->id == 1))
		ft_sleep_for_action(philo, philo->table->time_to_eat, "is thinking");
	while (!ft_check_end_diner(table))
	{
		if (table->n_philo == 1)
		{
			ft_sleep_for_action(philo, 0, "has taken a fork");
			break ;
		}
		ft_diner(philo);
		ft_handle_sleep(philo, table);
	}
	return (NULL);
}

static void	ft_diner(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->forks[philo->left_fork]);
	pthread_mutex_lock(&table->forks[philo->right_fork]);
	ft_sleep_for_action(philo, 0, "has taken a fork");
	ft_sleep_for_action(philo, 0, "has taken a fork");
	ft_sleep_for_action(philo, table->time_to_eat, "is eating");
	pthread_mutex_lock(&table->eat_mtx);
	philo->time_last_meal = ft_get_time();
	philo->count_meals++;
	pthread_mutex_unlock(&table->eat_mtx);
	pthread_mutex_unlock(&table->forks[philo->left_fork]);
	pthread_mutex_unlock(&table->forks[philo->right_fork]);
}

static void	ft_handle_sleep(t_philo *philo, t_table *table)
{
	ft_sleep_for_action(philo, table->time_to_sleep, "is sleeping");
	if (table->n_philo % 2)
		ft_sleep_for_action(philo, table->time_to_eat, "is thinking");
	else
		ft_sleep_for_action(philo, (table->time_to_die
				- (table->time_to_eat + table->time_to_sleep
					+ table->n_philo)), "is thinking");
}

void	ft_sleep_for_action(t_philo *philo, long long va_time_ms, char *action)
{
	long long	start;

	start = ft_get_time();
	pthread_mutex_lock(&philo->table->print_mtx);
	if (ft_check_end_diner(philo->table))
	{
		pthread_mutex_unlock(&philo->table->print_mtx);
		return ;
	}
	if (*action)
		printf("%lld %d %s\n", ft_get_time() - philo->table->start_diner,
			philo->id, action);
	pthread_mutex_unlock(&philo->table->print_mtx);
	if (va_time_ms > 0)
		while ((ft_get_time() - start < va_time_ms))
			usleep(100);
}
