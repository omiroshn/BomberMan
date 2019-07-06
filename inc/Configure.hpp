/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configure.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 11:58:20 by mrassokh          #+#    #+#             */
/*   Updated: 2019/06/28 11:58:23 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <fstream>
#include <iostream>
#include "GameWindow.hpp"
#include "Game.hpp"

#define CONFIGURATION Configure::getInstance()

int const DefaultScreenWidth = 960;
int const DefaultScreenHeight = 640;
int const DefaultChosenStage = 1;
int const DefaultScore = 100;
int const DefaultLives = 3;
int const DefaultMusicVolume = 5;
int const DefaultmSoundsVolume = 2;
int const DefaultKeyBindVolume = 0;
int const DefaultScreenResolution = 3;
bool const DefaultWindowed = false;

class Configure
{
public:
	static Configure   &getInstance();

	int 			getWidth() const;
	int 			getHeight() const;
	bool  			&getWindowed();
	int   			getChosenStage() const;
	int 			getScore() const;
	int 			getLives() const;
	int 			&getMusicVolume();
	int 			&getSoundVolume();
	int 			&getKeyBindVolume();
	int 			&getScreenResolution();

	void 			setWidth(int const w) ;
	void 			setHeight(int const h);
	void 			setSize(int const w, int const h);
	void 			setWindowed(bool const windowed);
	void 			setChosenStage(int const stage);
	void 			setScore(int const score);
	void 			setLives(int const lives);
	void 			setMusicVolume(int const musicVolume);
	void 			setSoundsVolume(int const soundsVolume);
	void 			setKeyBindVolume(int const keyBinding);
	void 			setScreenResolution(int const screenResolution);

	void 			serialise(std::string fileName  = "ConfigureFile");
	void 			deserialise(std::string fileName  = "ConfigureFile");

	void 			setObservableWindow(std::shared_ptr<GameWindow> gameWindow);

private:
	Configure();
	Configure(Configure const & rhs) = delete;
	Configure & operator=(Configure const & rhs) = delete;
	~Configure();
	//friend class boost::serialization::access;

public:
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar (mWindowWidth,
		mWindowHeight,
		mWindowed,
		mChosenStage,
		mScore,
		mLives,
		mMusicVolume,
		mSoundsVolume,
		mKeyBindVolume,
		mScreenResolution);
	}

private:
	int 			mWindowWidth;
	int 			mWindowHeight;
	bool  			mWindowed;
	int   			mChosenStage;
	int   			mScore;
	int  	 		mLives;
	int   			mMusicVolume;
	int   			mSoundsVolume;
	int   			mKeyBindVolume;
	int   			mScreenResolution;

	std::shared_ptr<GameWindow>      observableWindow;
};
