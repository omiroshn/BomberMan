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
	std::remove_if(v.begin(), v.end(), [](const std::unique_ptr<TimerBase>& timer){
		return timer->readyForDelete() && timer->usedTheEvent();
	});
}

void TimerManager::RemoveTimer(TimerBase *toDelete)
{
	std::remove_if(v.begin(), v.end(), [toDelete](const std::unique_ptr<TimerBase>& timer){
		return timer.get() == toDelete;
	});
}