/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 19:32:19 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 19:32:19 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include "Timer.h"
#include <algorithm>
#include <vector>

#include <iostream>
#include <memory>
#include "Core.hpp"

class TimerManager : public Tickable
{
private:
	std::vector<std::unique_ptr<TimerBase>> v;

public:
	static TimerManager *Instance();
	void Update();

	template<typename funcType>
	TimerBase *AddTimer(float seconds, bool ever, funcType type)
	{
		TimerBase *timer = new Timer<funcType>(seconds, ever, type);
		v.emplace_back(timer);
		return timer;
	}

	void RemoveTimer(TimerBase *timer);

	virtual void tick(float DeltaTime) override;
private:
	TimerManager();
	~TimerManager();
};

#endif