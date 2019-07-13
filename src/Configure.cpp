/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configure.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 15:39:50 by mrassokh          #+#    #+#             */
/*   Updated: 2019/06/28 15:39:52 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "Configure.hpp"
#include <cereal/cereal.hpp>
#include <cereal/details/helpers.hpp>
#include <cereal/archives/binary.hpp>

Configure   & Configure::getInstance()
{
	static Configure instance;
	return instance;
}

Configure::Configure():mWindowWidth(DefaultScreenWidth),
						mWindowHeight(DefaultScreenHeight),
						mWindowed(DefaultWindowed),
						mChosenStage(DefaultChosenStage),
						mScore(DefaultScore),
						mLives(DefaultLives),
						mMusicVolume(DefaultMusicVolume),
						mSoundsVolume(DefaultmSoundsVolume),
						mKeyBindVolume(DefaultKeyBindVolume),
						mScreenResolution(DefaultScreenResolution),
						mBombMax(DefaultBombMax),
						mBombStrength(DefaultBombStrength)
{

}


Configure::~Configure()
{

}

int 			Configure::getWidth() const
{
	return mWindowWidth;
}

int 			Configure::getHeight() const
{
	return mWindowHeight;
}

bool  			&Configure::getWindowed()
{
	return mWindowed;
}

int   			Configure::getChosenStage() const
{
	return mChosenStage;
}

int 			Configure::getScore() const
{
	return mScore;
}

int 			Configure::getLives() const
{
	return mLives;
}

int 			&Configure::getMusicVolume()
{
	return mMusicVolume;
}

int 			&Configure::getSoundVolume()
{
	return mSoundsVolume;
}

int 			&Configure::getKeyBindVolume()
{
	return mKeyBindVolume;
}

int 			&Configure::getScreenResolution()
{
	return mScreenResolution;
}

int             &Configure::getBombMax()
{
	return mBombMax;
}

int             &Configure::getBombStrength()
{
	return mBombStrength;
}

void 			Configure::setSize(int const w, int const h)
{
	setWidth(w);
	setHeight(h);
}

void 			Configure::setWidth(int const w)
{
	mWindowWidth = w;
}

void 			Configure::setHeight(int const h)
{
	mWindowHeight = h;
}

void 			Configure::setWindowed(bool const windowed)
{
	mWindowed = windowed;
	observableWindow->setFullScreen(mWindowed);
}

void 			Configure::setChosenStage(int const stage)
{
	mChosenStage = stage;
}

void 			Configure::setScore(int const score)
{
	mScore = score;
}

void 			Configure::setLives(int const lives)
{
	mLives = lives;
}

void 			Configure::setMusicVolume(int const musicVolume)
{
	mMusicVolume = 	musicVolume;
}

void 			Configure::setSoundsVolume(int const soundsVolume)
{
	mSoundsVolume = soundsVolume;
}

void 			Configure::setKeyBindVolume(int const keyBindVolume)
{
	mKeyBindVolume = keyBindVolume;
}

void 			Configure::setScreenResolution(int const screenResolution)
{
	mScreenResolution = screenResolution;
}

void            Configure::setBombMax(int const bombMax)
{
	mBombMax = bombMax;
}

void            Configure::setBombStrength(int const bombStrength)
{
	mBombStrength = bombStrength;
}

void 			Configure::serialise(std::string fileName)
{
	try
	{
		std::ofstream ofs(fileName);
		cereal::BinaryOutputArchive oa(ofs);
		oa << *this;
	} catch (std::exception &e) {
		std::cout << "configuration saving is failed due to " << e.what() << std::endl;
	}
}

void 			Configure::deserialise(std::string fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs.good())
		return;
	try
	{
		cereal::BinaryInputArchive ia(ifs);
		ia >> *this;
	} catch (std::exception &e) {
		std::cout << "configuration loading is failed due to " << e.what() << std::endl;
	}
}

void 			Configure::setObservableWindow(std::shared_ptr<GameWindow> gameWindow)
{
	observableWindow = gameWindow;
}
