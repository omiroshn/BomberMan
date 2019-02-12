/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:10:41 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/12 19:10:41 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.h"
#include <mach/mach_time.h>

void		init_timer(t_timer *timer)
{
	mach_timebase_info_data_t coeff;

	mach_timebase_info(&coeff);
	timer->numer = coeff.numer;
	timer->denom = coeff.denom;
	timer->current_time = 0;
	timer->start = mach_absolute_time();
	timer->current = timer->start;
	timer->time = 0.0;
	timer->old_time = 0.0;
}

uint64_t	get_updated_time(t_timer *timer)
{
	timer->current = mach_absolute_time() - timer->start;
	timer->current_time = timer->current * timer->numer / timer->denom;
	return (timer->current_time);
}

uint64_t	get_current_time(t_timer *timer)
{
	return (timer->current_time);
}

uint32_t	get_current_time_in_us(t_timer *timer)
{
	return (timer->current_time / 1000);
}

double		get_delta_time(t_timer *timer)
{
	double delta_time;

	timer->current = mach_absolute_time() - timer->start;
	timer->current_time = timer->current * timer->numer / timer->denom;
	timer->time = (double)(timer->current_time / 1000) / 1000000.0;
	delta_time = timer->time - timer->old_time;
	timer->old_time = timer->time;
	return (delta_time);
}
