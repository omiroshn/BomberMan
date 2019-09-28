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

void 			FfmpegPlayer::setBinFolder(std::string const& aPath)
{
	m_BinFolder = aPath + "/Assets/";
}

void 		FfmpegPlayer::setSDLWindow(std::shared_ptr<GameWindow>  window)
{
	m_window = window;
}

void 			FfmpegPlayer::registerCodecs()
{
	av_register_all();
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
	for (i = 0; i < m_pFormatCtx->nb_streams; ++i){
		if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_videoStream = i;
			break;
		}
	}
	if (m_videoStream == -1)
		throw CustomException("Didn't find a video stream");

	m_pCodecCtxOrig = m_pFormatCtx->streams[m_videoStream]->codec;
	// Find the decoder for the video stream
	m_pCodec = avcodec_find_decoder(m_pCodecCtxOrig->codec_id);
	if (m_pCodec == NULL) {
		throw CustomException("Unsupported codec!");
	}
	// Copy context
	m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	if (avcodec_copy_context(m_pCodecCtx, m_pCodecCtxOrig) != 0) {
		throw CustomException("Couldn't copy codec context!");
	}
	// Open codec
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0) {
		throw CustomException("Could not open codec!");
	}
}


void 			FfmpegPlayer::allocateFrame()
{
	// Allocate video frame
	m_pFrame = av_frame_alloc();
}

void 			FfmpegPlayer::createWindow()
{
	int x = 100, y = 100;

	m_window->getPosition(x,y);
	m_screen = m_window->getSDLWindow();
//	m_screen = SDL_CreateWindow(
//			"Bomberman",
//			x,
//			y,
//			CONFIGURATION.getWidth(),
//			CONFIGURATION.getHeight(),
//			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
//		);

	SDL_SetWindowResizable(m_screen, SDL_TRUE);
}

void 			FfmpegPlayer::createRenferer()
{
	m_renderer = SDL_CreateRenderer(m_screen, -1, 0);
	if (!m_renderer) {
		throw CustomException("SDL: could not create renderer!");
	}
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
			SWS_BILINEAR,
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
	int frameFinished;
	SDL_Event ev;

	uvPitch = m_pCodecCtx->width / 2;

	while (av_read_frame(m_pFormatCtx, &packet) >= 0) {
		// Is this a packet from the video stream?
		if (packet.stream_index == m_videoStream) {
			// Decode video frame
			avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, &packet);

			// Did we get a video frame?
			if (frameFinished) {
				AVPicture pict;
				pict.data[0] = m_yPlane;
				pict.data[1] = m_uPlane;
				pict.data[2] = m_vPlane;
				pict.linesize[0] = m_pCodecCtx->width;
				pict.linesize[1] = uvPitch;
				pict.linesize[2] = uvPitch;

				// Convert the image into YUV format that SDL uses
				sws_scale(m_sws_ctx, (uint8_t const * const *) m_pFrame->data,
						m_pFrame->linesize, 0, m_pCodecCtx->height, pict.data,
						pict.linesize);

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

				//updateBaseWindowSizePos();
				SDL_Delay(videoFrameDelay);
			}
		}
		SDL_PollEvent(&m_event);
		if (m_event.type == SDL_QUIT || (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_ESCAPE))
		{
			destroyVideoSession();
			CONFIGURATION.serialise();
			SDL_Quit();
			exit(0);
			break;
		}
		av_free_packet(&packet);
	}
	av_free_packet(&packet);
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
	avcodec_close(m_pCodecCtxOrig);

	// Close the video file
	avformat_close_input(&m_pFormatCtx);
	sws_freeContext(m_sws_ctx);
}

void 			FfmpegPlayer::resetGLContext()
{
	//SDL_GL_MakeCurrent(m_window->getSDLWindow(), m_window->getSDLGLContext());
}

void 			FfmpegPlayer::playVideo(std::string const &source)
{
	try {
		openVideoFile(source);
		findCodec();
		allocateFrame();
		createWindow();
		createRenferer();
		createTexture();
		initSWSontext();
		allocatePixelArray();
		renderVideo();
		freeMemory();
		destroyVideoSession();
		resetGLContext();
		std::cout << "video form source " << source << "was played "<< std::endl;
	} catch (std::exception& e) {
		std::cerr << "Error:" << e.what() <<  " when loading video " << source << std::endl;
	}
}

void 			FfmpegPlayer::destroyVideoSession()
{
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	//SDL_DestroyWindow(m_screen);
}

void 			FfmpegPlayer::updateBaseWindowSizePos()
{
	int width, height;
	int x, y;

	SDL_GetWindowSize(m_screen, &width, &height);
	m_window->setSize(width, height);

	SDL_GetWindowPosition(m_screen, &x, &y);
	m_window->setPosition(x, y);

	CONFIGURATION.setHeight(height);
	CONFIGURATION.setWidth(width);
}
