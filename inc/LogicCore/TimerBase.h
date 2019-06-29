/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerBase.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 16:38:46 by omiroshn          #+#    #+#             */
/*   Updated: 2019/06/29 16:38:47 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_BASE_H
#define TIMER_BASE_H

class TimerBase
{
public:
    virtual void Tick() = 0;
	virtual bool readyForDelete() = 0;
	virtual bool usedTheEvent() = 0;
};

#endif
