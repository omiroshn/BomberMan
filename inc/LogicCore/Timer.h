/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 17:43:45 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 17:43:46 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
#define TIMER_H

#include "TimerBase.h"
#include "Game.hpp"

template <typename funcType>
class Timer : public TimerBase
{
 private:
	float duration;
	float endTime;

	funcType event;

public:
	Timer(float seconds, bool everlasting, funcType lambda) : event(lambda) {
		float startTime = Game::getCurrentTime();
		duration = seconds;
		endTime = startTime + seconds;
		toDelete = !everlasting;
		usedEvent = false;
	}

	Timer() = delete;
	Timer(Timer const&) = delete;
    Timer& operator=(Timer const&) = delete;

	void Tick() {
		if (Game::getCurrentTime() >= endTime)
		{
			event();
			endTime += duration;
			usedEvent = true;
		}
	}

	virtual ~Timer() {}
};

#endif
