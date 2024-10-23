/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:44:26 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/10/23 00:08:42 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;

typedef struct s_philo
{
	int			id;
	int			right_fork;
	int			left_fork;
	long long	count_meals;
	long long	time_last_meal;
	pthread_t	thread;
	t_table		*table;

}			t_philo;

typedef struct s_table
{
	t_philo		*philo;
	int			n_philo;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	long long	n_meals;
	int			end_diner;
	long long	start_diner;
	int			n_philos_full;
	t_mtx		*forks;
	t_mtx		start_mtx;
	t_mtx		eat_mtx;
	t_mtx		end_mtx;
	t_mtx		print_mtx;
}				t_table;

void		ft_init_philo_threads(t_table *table);
void		*ft_start_diner(void *data);
void		ft_sleep_for_action(t_philo *philo, long long va_time_ms,
				char *action);
void		ft_start_monitor(t_table *table);
void		ft_handle_error(char *str, t_table *table, int error_stage);
int			ft_atoi(const char *ptr);
long long	ft_atoll(char *str, int i, long long res);
long long	ft_get_time(void);
int			ft_check_end_diner(t_table *table);
