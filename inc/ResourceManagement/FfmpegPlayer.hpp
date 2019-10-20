/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FfmpegPlayer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 13:23:31 by mrassokh          #+#    #+#             */
/*   Updated: 2019/08/04 13:23:33 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavformat/avio.h"
    #include "libavutil/file.h"
	#include <libswscale/swscale.h>
}

#include <iostream>
#include <SDL.h>
#include <GameWindow.hpp>

#define FFMPEG FfmpegPlayer::getInstance()
using gameWinPtr = std::shared_ptr<GameWindow>;

class FfmpegPlayer{
public:
	static FfmpegPlayer 	&getInstance();

	void 			setBinFolder(std::string const& aPath);
	void 			setSDLWindow(std::shared_ptr<GameWindow>  window);
	void 			playVideo(std::string const &source);
	void			init(std::shared_ptr<GameWindow>  window);

private:
	FfmpegPlayer();
	FfmpegPlayer(FfmpegPlayer const & rhs) = delete;
	FfmpegPlayer& operator=(FfmpegPlayer const & rhs) = delete;
	~FfmpegPlayer();

	void 			openVideoFile(std::string const &source);
	void 			findCodec();
	void 			allocateFrame();
	void 			createTexture();
	void 			initSWSontext();
	void 			allocatePixelArray();
	void 			renderVideo();
	void 			freeMemory();
	void 			resetGLContext();
	void 			destroyVideoSession();

private:
	AVFormatContext 	*m_pFormatCtx = NULL;
	AVCodecContext 		*m_pCodecCtx = NULL;
	AVCodec 			*m_pCodec = NULL;

	AVFrame 			*m_pFrame = NULL;
	SDL_Renderer 		*m_renderer = NULL;
	SDL_Texture 		*m_texture = NULL;

	Uint8 				*m_yPlane = NULL;
	Uint8  				*m_uPlane = NULL;
	Uint8  				*m_vPlane = NULL;

	struct SwsContext 	*m_sws_ctx = NULL;
	int 				m_videoStream;
	gameWinPtr 			m_window;
	std::string      	m_BinFolder;

};
