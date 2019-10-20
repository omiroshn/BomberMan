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
#include "Configure.hpp"
#include "Game.hpp"
#include "CustomException.hpp"

static const Uint32 videoFrameDelay = 40;

FfmpegPlayer 	&FfmpegPlayer::getInstance()
{
	static FfmpegPlayer instance;
	return instance;
}

FfmpegPlayer::FfmpegPlayer() : m_videoStream(-1)
{

}

FfmpegPlayer::~FfmpegPlayer()
{
}

void FfmpegPlayer::init(std::shared_ptr<GameWindow>  window)
{
	setSDLWindow(window);
	m_renderer = SDL_CreateRenderer(m_window->getSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_renderer)
	{
		throw CustomException("SDL: could not create renderer!");
	}
}

void 			FfmpegPlayer::setBinFolder(std::string const& aPath)
{
	m_BinFolder = aPath + "/Assets/";
}

void 		FfmpegPlayer::setSDLWindow(std::shared_ptr<GameWindow>  window)
{
	m_window = window;
}

void 			FfmpegPlayer::openVideoFile(std::string const &source)
{
	// Open video file
	if (avformat_open_input(&m_pFormatCtx, (m_BinFolder + "video/" + source).c_str(), NULL, NULL) != 0) {
		throw CustomException("Could not open music from source" + source);
	}
	// Retrieve stream information
	if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0) {
		throw CustomException("Couldn't find stream information from source" + source);
	}
}

void 			FfmpegPlayer::findCodec()
{
	m_videoStream = -1;
	unsigned i;

	// Find the first video stream
	for (i = 0; i < m_pFormatCtx->nb_streams; ++i)
	{
		if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_videoStream = i;
			break;
		}
	}
	if (m_videoStream == -1)
		throw CustomException("Didn't find a video stream");

    AVCodecParameters *codecpar = m_pFormatCtx->streams[m_videoStream]->codecpar;
	// Find the decoder for the video stream
	m_pCodec = avcodec_find_decoder(codecpar->codec_id);
	if (m_pCodec == NULL) {
		throw CustomException("Unsupported codec!");
	}
	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	// Copy context params
	if (avcodec_parameters_to_context(m_pCodecCtx, codecpar))
	{
		std::cerr << "Couldn't copy codec context!\n";
	}
	// Open codec
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
	{
		throw CustomException("Could not open codec!");
	}
}


void 			FfmpegPlayer::allocateFrame()
{
	// Allocate video frame
	m_pFrame = av_frame_alloc();
}

void 			FfmpegPlayer::createTexture()
{
	// Allocate a place to put our YUV image on that screen
	m_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_YV12,
		SDL_TEXTUREACCESS_STREAMING,
		m_pCodecCtx->width,
		m_pCodecCtx->height
	);
	if (!m_texture) {
		throw CustomException("SDL: could not create texture!");
	}
}

void 			FfmpegPlayer::initSWSontext()
{
	// initialize SWS context for software scaling
	m_sws_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height,
			m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height,
			AV_PIX_FMT_YUV420P,
			SWS_BICUBIC,
			NULL,
			NULL,
			NULL);
}

void 			FfmpegPlayer::allocatePixelArray()
{
	size_t yPlaneSz, uvPlaneSz;

	// set up YV12 pixel array (12 bits per pixel)
	yPlaneSz = m_pCodecCtx->width * m_pCodecCtx->height;
	uvPlaneSz = m_pCodecCtx->width * m_pCodecCtx->height / 4;
	m_yPlane = (Uint8*)malloc(yPlaneSz);
	m_uPlane = (Uint8*)malloc(uvPlaneSz);
	m_vPlane = (Uint8*)malloc(uvPlaneSz);
	if (!m_yPlane || !m_uPlane || !m_vPlane) {
		throw CustomException("Could not allocate pixel buffers");
	}
}

void 			FfmpegPlayer::renderVideo()
{
	int uvPitch;
	AVPacket packet;

	uvPitch = m_pCodecCtx->width / 2;
	Uint32 lastTime = SDL_GetTicks();

	while (av_read_frame(m_pFormatCtx, &packet) >= 0)
	{
		// Is this a packet from the video stream?
		if (packet.stream_index == m_videoStream)
		{
			//avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, &packet);
			// Decode video frame
			avcodec_send_packet(m_pCodecCtx, &packet);
			int recieveStatus = avcodec_receive_frame(m_pCodecCtx, m_pFrame);

			// Did we get a video frame?
			if (recieveStatus == 0)
			{
				AVFrame frame;
				frame.data[0] = m_yPlane;
				frame.data[1] = m_uPlane;
				frame.data[2] = m_vPlane;
				frame.linesize[0] = m_pCodecCtx->width;
				frame.linesize[1] = uvPitch;
				frame.linesize[2] = uvPitch;

				// Convert the image into YUV format that SDL uses
				sws_scale(m_sws_ctx, (uint8_t const * const *) m_pFrame->data,
						m_pFrame->linesize, 0, m_pCodecCtx->height, frame.data,
						frame.linesize);

				SDL_UpdateYUVTexture(
						m_texture,
						NULL,
						m_yPlane,
						m_pCodecCtx->width,
						m_uPlane,
						uvPitch,
						m_vPlane,
						uvPitch
					);

				SDL_RenderClear(m_renderer);
				SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
				SDL_RenderPresent(m_renderer);

				resetGLContext();
				bool worked = RESOURCES.tickLoading();

				Uint32 currentTime = SDL_GetTicks();
				Uint32 timeToSleep = videoFrameDelay;
				if (worked)
				{
					timeToSleep = videoFrameDelay - (currentTime - lastTime);
					if (timeToSleep > videoFrameDelay)
						timeToSleep = 40;
				}
				lastTime = currentTime;
				SDL_Delay(timeToSleep);
			}
		}

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				Game::get()->requestExit();
				return;
			}
		}
		av_packet_unref(&packet);
	}
	av_packet_unref(&packet);
}

void 			FfmpegPlayer::freeMemory()
{
	// Free the YUV frame
	av_frame_free(&m_pFrame);
	free(m_yPlane);
	free(m_uPlane);
	free(m_vPlane);

	// Close the codec
	avcodec_free_context(&m_pCodecCtx);
	avcodec_close(m_pCodecCtx);

	// Close the video file
	avformat_close_input(&m_pFormatCtx);
	sws_freeContext(m_sws_ctx);
}

void 			FfmpegPlayer::resetGLContext()
{
	SDL_GL_MakeCurrent(m_window->getSDLWindow(), m_window->getSDLGLContext());
}

void 			FfmpegPlayer::playVideo(std::string const &source)
{
	try {
		openVideoFile(source);
		findCodec();
		allocateFrame();
		createTexture();
		initSWSontext();
		allocatePixelArray();
		renderVideo();
		freeMemory();
		destroyVideoSession();
		resetGLContext();
	} catch (std::exception& e) {
		std::cerr << "Error:" << e.what() <<  " when loading video " << source << std::endl;
	}
}

void 			FfmpegPlayer::destroyVideoSession()
{
	SDL_DestroyTexture(m_texture);
}
