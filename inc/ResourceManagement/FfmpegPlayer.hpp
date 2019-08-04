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

#define FFMPEG FfmpegPlayer::getInstance()

class FfmpegPlayer{
public:
	static FfmpegPlayer 	&getInstance();

	void 			setBinFolder(std::string const& aPath);
	void 			loadVideo(std::string const &videoName, std::string const &source);

	AVFormatContext *m_pFormatCtx;
	AVCodecContext 	*m_pCodecCtx;
	AVCodec 		*m_pCodec;
private:
	FfmpegPlayer();
	FfmpegPlayer(FfmpegPlayer const & rhs) = delete;
	FfmpegPlayer& operator=(FfmpegPlayer const & rhs) = delete;
	~FfmpegPlayer();

	std::string      mBinFolder;

};
