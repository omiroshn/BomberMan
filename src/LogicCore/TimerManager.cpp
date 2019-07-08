/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 19:32:22 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 19:32:28 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/TimerManager.h"

TimerManager *TimerManager::sInstance = nullptr;

TimerManager *TimerManager::Instance() {

	if (sInstance == nullptr)
		sInstance = new TimerManager();

	return sInstance;
}

TimerManager::TimerManager() {}
TimerManager::~TimerManager() {}

void TimerManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

void TimerManager::Update() {
	for (auto& it : v)
	{
		it->Tick();
	}
	v.erase(std::remove_if(v.begin(), v.end(), [](std::unique_ptr<TimerBase>& timer){
		return timer->readyForDelete() && timer->usedTheEvent();
	}), v.end());
}

void TimerManager::RemoveTimer(TimerBase *toDelete)
{
	v.erase(std::remove_if(v.begin(), v.end(), [toDelete](const std::unique_ptr<TimerBase>& timer){
		return timer.get() == toDelete;
	}), v.end());
}