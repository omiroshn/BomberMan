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
	bool toDelete;
	bool usedEvent;

    virtual void Tick() = 0;
	virtual bool readyForDelete() {
		return toDelete;
	}
	virtual bool usedTheEvent() {
		return usedEvent;
	}
	virtual ~TimerBase(){};
};

#endif
