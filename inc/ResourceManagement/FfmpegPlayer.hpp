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
	void 			registerCodecs();
	void 			setSDLWindow(std::shared_ptr<GameWindow>  window);
	void 			playVideo(std::string const &source);

private:
	FfmpegPlayer();
	FfmpegPlayer(FfmpegPlayer const & rhs) = delete;
	FfmpegPlayer& operator=(FfmpegPlayer const & rhs) = delete;
	~FfmpegPlayer();

	void 			openVideoFile(std::string const &source);
	void 			findCodec();
	void 			allocateFrame();
	void 			createWindow();
	void 			createRenferer();
	void 			createTexture();
	void 			initSWSontext();
	void 			allocatePixelArray();
	void 			renderVideo();
	void 			freeMemory();
	void 			resetGLContext();
	void 			destroyVideoSession();

	void 			updateBaseWindowSizePos();

private:
	AVFormatContext 	*m_pFormatCtx;
	AVCodecContext 		*m_pCodecCtx;
	AVCodecContext 		*m_pCodecCtxOrig;
	AVCodec 			*m_pCodec;

	AVFrame 			*m_pFrame;
	AVFrame 			*m_pFrameRGB;
	SDL_Window 			*m_screen;
	SDL_Renderer 		*m_renderer;
	SDL_Texture 		*m_texture;

	Uint8 				*m_yPlane;
	Uint8  				*m_uPlane;
	Uint8  				*m_vPlane;

	struct SwsContext 	*m_sws_ctx;
	SDL_Event 			m_event;
	int 				m_videoStream;
	gameWinPtr 			m_window;
	std::string      	m_BinFolder;

};
