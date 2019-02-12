/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:10:28 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/12 19:10:29 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
# define TIMER_H

# include <stdint.h>

typedef struct	s_timer
{
	uint64_t	start;
	uint64_t	current;
	uint64_t	current_time;
	uint64_t	numer;
	uint64_t	denom;
	double		time;
	double		old_time;
}				t_timer;

void			init_timer(t_timer *timer);
uint64_t		get_current_time(t_timer *timer);
uint64_t		get_updated_time(t_timer *timer);
uint32_t		get_current_time_in_us(t_timer *timer);
double			get_delta_time(t_timer *timer);

#endif
