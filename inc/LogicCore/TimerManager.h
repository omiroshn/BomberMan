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
#include <vector>
#include <iostream>

class TimerManager
{
private:
	static TimerManager* sInstance;
	std::vector<TimerBase *> v;
public:
	static TimerManager *Instance();
	static void Release();
	void Update();

	template<typename funcType>
	void AddTimer(float seconds, bool ever, funcType type)
	{
		auto timer = new Timer<funcType>(seconds * 1000, ever, type);
		v.push_back(timer);
	}

private:
	TimerManager();
	~TimerManager();
};

#endif
