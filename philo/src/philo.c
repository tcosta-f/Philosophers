/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:55:06 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 01:02:39 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int			main(int argc, char **argv);
static int	ft_parsing_args_to_table(t_table *table, char **argv);
static void	ft_parsing_philo_and_forks(t_table *table);
static void	ft_init_mutexes(t_table *table);
static void	ft_init_philo(t_table *table, int i);

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
		ft_handle_error("Error\nUsage: <number_of_philosophers>"
			"<time_to_die> <time_to_eat> <time_to_sleep>"
			"[number_of_times_each_philosopher_must_eat]\n", NULL, 0);
	ft_parsing_args_to_table(&table, argv);
	ft_parsing_philo_and_forks(&table);
	ft_init_philo_threads(&table);
	ft_start_monitor(&table);
	ft_handle_error(NULL, &table, 3);
	return (0);
}

static int	ft_parsing_args_to_table(t_table *table, char **argv)
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoll(argv[2], 0, 0);
	table->time_to_eat = ft_atoll(argv[3], 0, 0);
	table->time_to_sleep = ft_atoll(argv[4], 0, 0);
	if (table->n_philo < 1 || table->time_to_die < 1
		|| table->time_to_eat < 1 || table->time_to_sleep < 1)
		ft_handle_error("Error\nTry values bigger than 0!\n", NULL, 0);
	if (argv[5])
	{
		table->n_meals = ft_atoll(argv[5], 0, 0);
		if (table->n_meals <= 0)
			ft_handle_error("Error\nTry values bigger than 0!\n", NULL, 0);
	}
	else
		table->n_meals = -1;
	table->n_philos_full = 0;
	table->start_diner = ft_get_time();
	table->end_diner = 0;
	table->philo = NULL;
	table->forks = NULL;
	return (0);
}

static void	ft_parsing_philo_and_forks(t_table *table)
{
	int	i;

	i = 0;
	table->philo = malloc(sizeof(t_philo) * table->n_philo);
	if (table->philo == NULL)
		ft_handle_error("Error\nmalloc fails\n", table, 0);
	table->forks = malloc(sizeof(t_mtx) * table->n_philo);
	if (table->forks == NULL)
		ft_handle_error("Error\nmalloc fails\n", table, 1);
	while (i < table->n_philo)
	{
		ft_init_philo(table, i);
		i++;
	}
	ft_init_mutexes(table);
}

static void	ft_init_philo(t_table *table, int i)
{
	table->philo[i].id = i + 1;
	table->philo[i].right_fork = i;
	table->philo[i].left_fork = (i + 1) % table->n_philo;
	table->philo[i].table = table;
	table->philo[i].count_meals = 0;
	table->philo[i].time_last_meal = table->start_diner;
	if (pthread_mutex_init(&table->forks[i], NULL))
		ft_handle_error("Error\nMutex init fails\n", table, 1);
}

static void	ft_init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->start_mtx, NULL))
		ft_handle_error("Error\nMutex failed\n", table, 1);
	if (pthread_mutex_init(&table->end_mtx, NULL))
		ft_handle_error("Error\nMutex failed\n", table, 1);
	if (pthread_mutex_init(&table->print_mtx, NULL))
		ft_handle_error("Error\nMutex failed\n", table, 1);
	if (pthread_mutex_init(&table->eat_mtx, NULL))
		ft_handle_error("Error\nMutex failed\n", table, 1);
}
