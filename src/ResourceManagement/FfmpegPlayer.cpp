/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FfmpegPlayer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 13:23:09 by mrassokh          #+#    #+#             */
/*   Updated: 2019/08/04 13:23:11 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResourceManagement/FfmpegPlayer.hpp"

FfmpegPlayer 	&FfmpegPlayer::getInstance()
{
	static FfmpegPlayer instance;
	return instance;
}

FfmpegPlayer::FfmpegPlayer()
{
	//setBinFolder()
	//loadVideo("videoName","drop.avi");
}

FfmpegPlayer::~FfmpegPlayer()
{

}


void 			FfmpegPlayer::setBinFolder(std::string const& aPath)
{
	mBinFolder = aPath + "/Assets/";
}

void 			FfmpegPlayer::loadVideo(std::string const &videoName, std::string const &source)
{
	//AVFormatContext *m_pFormatCtx = NULL;
	m_pFormatCtx = NULL;

// Open video file
std::cout << "debug_0" << std::endl;
	if(avformat_open_input(&m_pFormatCtx, (mBinFolder + "video/" + source).c_str(), NULL, NULL)!=0)
	{
		std::cout << "Could not open music with name " << videoName << " from source "
			<< (mBinFolder + "video/" + source)<<std::endl;
			return;
	}
	std::cout << "debug_1" << std::endl;

	// Retrieve stream information
	if(avformat_find_stream_info(m_pFormatCtx, NULL)<0)
	{
		std::cout << "Couldn't find stream information for " << videoName << " from source "
			<< (mBinFolder + "video/" + source)<<std::endl;
			return;
	}
	std::cout << "debug_2" << std::endl;

	av_dump_format(m_pFormatCtx, 0, source.c_str(), 0);
	std::cout << "debug_3" << std::endl;


	int i;
	AVCodecContext *m_pCodecCtxOrig = NULL;
	AVCodecContext *m_pCodecCtx = NULL;

	// Find the first video stream
	int videoStream=-1;
		std::cout << "debug_4" << std::endl;
	for(i = 0; i < m_pFormatCtx->nb_streams; ++i)
  		if(m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
    		videoStream = i;
    		break;
  		}
	if(videoStream==-1)
	{
		std::cout << "Didn't find a video stream " << videoName << " from source "
			<< (mBinFolder + "video/" + source)<<std::endl;
			return;
	}
	if (videoStream == m_pFormatCtx->nb_streams) {
	fprintf(stderr, "ffmpeg: Unable to find video stream\n");
	return ;
}

	// Get a pointer to the codec context for the video stream
	m_pCodecCtx=m_pFormatCtx->streams[videoStream]->codec;
	std::cout << "debug_5" << std::endl;
	m_pCodec = NULL;

	// Find the decoder for the video stream
	m_pCodec=avcodec_find_decoder(m_pCodecCtx->codec_id);
	if(m_pCodec==NULL) {
  		fprintf(stderr, "Unsupported codec!\n");
  		return ; // Codec not found
	}
		std::cout << "debug_6" << std::endl;
	// Copy context
	//m_pCodecCtx = avcodec_alloc_context3(pCodec);
	std::cout << "debug_6_1" << std::endl;
	/*if(avcodec_copy_context(m_pCodecCtx, m_pCodecCtxOrig) != 0) {
  		fprintf(stderr, "Couldn't copy codec context");
  		return ; // Error copying codec context
	}*/
		std::cout << "debug_7" << std::endl;
	// Open codec
	if(avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
  		return ; // Could not open codec

}
