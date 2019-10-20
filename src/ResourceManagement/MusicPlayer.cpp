/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MusicPlayer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 12:24:44 by mrassokh          #+#    #+#             */
/*   Updated: 2019/08/06 22:46:22 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/MusicPlayer.hpp"
#include "Configure.hpp"

MusicPlayer 	&MusicPlayer::getInstance()
{
	static MusicPlayer  instance;
	return instance;
}

MusicPlayer::MusicPlayer() : mBinFolder("")
{

}

MusicPlayer::~MusicPlayer()
{
	clearMusicMap();
	clearSoundMap();
	Mix_Quit();
}

void 			MusicPlayer::setBinFolder(std::string const& aPath)
{
	mBinFolder = aPath + "/Assets/";
}

void 			MusicPlayer::initLoad()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	}
	try {
		loadMusic("tango","tango_siciliana_proud.wav");
		loadMusic("candyman","candyman.wav");
		loadSound("explosion","explosion.wav");
		loadSound("death","death.wav");
		loadSound("bonus","bonus.wav");
		loadSound("bomberman","bomberman.wav");
		loadSound("ugh","ugh.wav");
	} catch (std::exception &e) {
		std::cout << e.what();
	}
}

void 			MusicPlayer::loadMusic(std::string const &musicName, std::string const &source)
{
	Mix_Music* musicPtr = Mix_LoadMUS((mBinFolder + "music/" + source).c_str());
	if (!musicPtr){
		return;
	}
	mMusic.emplace(musicName, musicPtr);
}

void 			MusicPlayer::loadSound(std::string const &soundName, std::string const &source)
{
	Mix_Chunk* soundPtr = Mix_LoadWAV((mBinFolder + "music/" + source).c_str());
	if (!soundPtr){
		return;
	}
	mSound.emplace(soundName, soundPtr);
}

void 			MusicPlayer::clearMusicMap()
{
	for (auto & element : mMusic)
	{
		Mix_FreeMusic(element.second);
		element.second = nullptr;
	}
	mMusic.clear();
}

void 			MusicPlayer::clearSoundMap()
{
	for (auto & element : mSound)
	{
		Mix_FreeChunk(element.second);
		element.second = nullptr;
	}
	mSound.clear();
}

void 			MusicPlayer::playMusicInfinity(std::string const &musicName) const
{
	auto music = mMusic.find(musicName);
	if (music != mMusic.end()){
		Mix_PlayMusic(music->second, -1);
		Mix_VolumeMusic(MIX_MAX_VOLUME * CONFIGURATION.getMusicVolume() / 10);
	} else {
		//std::cout << "Not found music with name " << musicName << std::endl;
	}

}
void 			MusicPlayer::playMusicOnce(std::string const &musicName)
{
	auto music = mMusic.find(musicName);
	if (music != mMusic.end()) {
		Mix_PlayMusic(music->second, 0);
		Mix_VolumeMusic(MIX_MAX_VOLUME * CONFIGURATION.getMusicVolume() / 10);
	} else {
		//std::cout << "Not found music with name " << musicName << std::endl;
	}
}

void 			MusicPlayer::pauseMusic() const
{
	Mix_PauseMusic();
}

void 			MusicPlayer::unPauseMusic() const
{
	Mix_ResumeMusic();
}

void 			MusicPlayer::playSound(std::string const &soundName) const
{
	auto sound = mSound.find(soundName);
	if (sound != mSound.end()) {
		Mix_PlayChannel(-1, sound->second, 0);
		Mix_Volume(-1, MIX_MAX_VOLUME * CONFIGURATION.getSoundVolume() / 10);
	} else {
		std::cout << "Not found sound with name " << soundName << std::endl;
	}
}
