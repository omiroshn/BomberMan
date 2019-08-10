/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MusicPlayer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:25:02 by mrassokh          #+#    #+#             */
/*   Updated: 2019/07/12 12:25:04 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include "CustomException.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#define MUSIC_PLAYER MusicPlayer::getInstance()
typedef std::map<std::string, Mix_Music*> musicMap;
typedef std::map<std::string, Mix_Chunk*> soundMap;
typedef std::vector<Mix_Music*> musList;

class MusicPlayer
{
public:
	static MusicPlayer 	&getInstance();

	void 				setBinFolder(std::string const& aPath);

	void 				initLoad();
	void 				loadMusic(std::string const &musicName, std::string const &source);
	void 				loadSound(std::string const &soundName, std::string const &source);

	void 				clearMusicMap();
	void 				clearSoundMap();

	void 				playMusicInfinity(std::string const &musicName) const;
	void 				playMusicOnce(std::string const &musicName);
	void 				pauseMusic() const;
	void 				unPauseMusic() const;

	void 				playSound(std::string const &soundName) const;

private:
	MusicPlayer();
	MusicPlayer(MusicPlayer const & rhs) = delete;
	MusicPlayer& operator=(MusicPlayer const & rhs) = delete;
	~MusicPlayer();

	musicMap 			mMusic;
	soundMap 			mSound;
	std::string			mBinFolder;
};
