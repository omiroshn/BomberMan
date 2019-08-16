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
	if(SDL_Init(SDL_INIT_AUDIO)==-1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	}
	try {
		loadMusic("tango","tango_siciliana_proud.wav");
		loadMusic("candyman","candyman.wav");
		loadSound("explosion","explosion.wav");
	} catch (std::exception &e) {
		std::cout << e.what();
	}
}

void 			MusicPlayer::loadMusic(std::string const &musicName, std::string const &source)
{
	Mix_Music* musicPtr = Mix_LoadMUS((mBinFolder + "music/" + source).c_str());
	if (!musicPtr){
		std::cout << "Could not open music with name " << musicName << " from source "
		<< (mBinFolder + "music/" + source)<<std::endl;
		return;
	}
	mMusic.emplace(musicName, musicPtr);
	std::cout << "Open music with name " << musicName << std::endl;
}

void 			MusicPlayer::loadSound(std::string const &soundName, std::string const &source)
{
	Mix_Chunk* soundPtr = Mix_LoadWAV((mBinFolder + "music/" + source).c_str());
	if (!soundPtr){
		std::cout << "Could not open sound with name " << soundName << " from source "
		<< (mBinFolder + "music/" + source)<<std::endl;
		return;
	}
	mSound.emplace(soundName, soundPtr);
	std::cout << "Open sound with name " << soundName << std::endl;
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
	} else {
		//std::cout << "Not found music with name " << musicName << std::endl;
	}

}
void 			MusicPlayer::playMusicOnce(std::string const &musicName)
{
	auto music = mMusic.find(musicName);
	if (music != mMusic.end()) {
		Mix_PlayMusic(music->second, 0);
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
	} else {
		std::cout << "Not found sound with name " << soundName << std::endl;
	}
}
