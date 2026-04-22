//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------


#include "RingBuffer.h"
#include "MediaFile.h"
#include "SystemHelper.h"
#include "AudioManager.h"
#include <libsysmodule.h>
#include <string.h>

extern "C"
{
	#include "libavutil/opt.h"
}

clMediaFile::stAudioParams clMediaFile::m_audioParams;

// When exiting a project we want to clean up exiting decoding happening in the tumbnail thread
// as quickly as possible. This flag when set short circuits decoding getting it to return as quickly as possible.
// We don't really care if decoding was successful. Added for Patch 3.03.
bool clMediaFile::m_bShortCircuitDecoding = false;

// Codec to thread count map
std::map<std::string, int> clMediaFile::m_mapCodecToThreadCount;

// Small pool of video contexts
clVideoContextWrapper s_videoContextWrapper;

void clMediaFile::PrintAVError(int err, std::string from)
{
	char errorBuffer[AV_ERROR_MAX_STRING_SIZE];
	av_strerror(err, errorBuffer, AV_ERROR_MAX_STRING_SIZE);
	NM_TRACESIMPLE("ERROR:  Unhandled ffmpeg error:  File: %s, Location: %s, Error: %i = %s\n", GetFilename().c_str(), from.c_str(), err, errorBuffer);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------

int clMediaFile::Initialize(stInitParams& initParams)
{
	// Setup logging
	FFMPEGUtils::InitializeLogging(initParams.pTraceFP, initParams.traceLevel);

	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::Initialize()\n");

	m_audioParams = initParams.initAudioParams;

	// Initialize the SCEA memory allocators
	int ret = InitializeSCEAMemory(&initParams.initParamsMemSCEA);
	if (ret == 0)
	{
		FFMPEGUtils::Log(AV_LOG_INFO, "InitializeSCEAMemory() Succeeded\n");
	}
	else
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "InitializeSCEAMemory() Returned: 0x%x\n", ret);
	}

	if (ret == 0)
	{
		// Initialize the SCEA file protocol
		ret = InitializeFileSCEA(&initParams.initParamsFileSCEA);
		if (ret == 0)
		{
			FFMPEGUtils::Log(AV_LOG_INFO, "InitializeFileSCEA() Succeeded\n");
		}
		else
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "InitializeFileSCEA() Returned: 0x%x\n", ret);
		}
	}

	if (ret == 0)
	{
		// Initialize the SCEA pthread code
		ret = InitializeSCEPThread(&initParams.initParamsSCEPThread);
		if (ret == 0)
		{
			FFMPEGUtils::Log(AV_LOG_INFO, "InitializeSCEPThread() Succeeded\n");
		}
		else
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "InitializeSCEPThread() Returned: 0x%x\n", ret);
		}
	}

	if (ret == 0)
	{
		if (ret != 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Initialization error Returned: 0x%x\n", ret);
		}
		else
		{
			// Initialize the H264 SCEA codec
			ret = InitializeH264SCEA(&initParams.initParamsH264SCEA);
			if (ret == 0)
			{
				FFMPEGUtils::Log(AV_LOG_INFO, "InitializeH264SCEA() Succeeded\n");
			}
			else
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "InitializeH264SCEA() Returned: 0x%x\n", ret);
			}
		}
	}

	m_mapCodecToThreadCount = initParams.mapCodecToThreadCount;

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
nmThread::clCriticalSectionSimple clMediaFile::m_criticalSectionMediaFile;

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
double clMediaFile::ConvertVideoPtsToMS(int64_t nPts) const
{
	NM_ASSERT(m_nVideoStreamIdx >= 0);
	double nSecs = double(nPts) * av_q2d(m_pFormatContext->streams[m_nVideoStreamIdx]->time_base);
	return nSecs * 1000.0;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
double clMediaFile::ConvertAudioPtsToMS(int64_t nPts) const
{
	NM_ASSERT(m_nAudioStreamIdx >= 0);
	return 1000.0 * double(nPts) * av_q2d(m_pFormatContext->streams[m_nAudioStreamIdx]->time_base);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
double clMediaFile::GetVideoPtsToMS( ) const
{
	NM_ASSERT(m_nVideoStreamIdx >= 0);
	return 1000.0 * av_q2d(m_pFormatContext->streams[m_nVideoStreamIdx]->time_base);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
double clMediaFile::GetAudioPtsToMS( ) const
{
	NM_ASSERT(m_nAudioStreamIdx >= 0);
	return 1000.0 * av_q2d(m_pFormatContext->streams[m_nAudioStreamIdx]->time_base);
}

//----------------------------------------------------------------------------------------------------------------------
// Validate the video attributes, bframes, etc.
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eVideoStatus clMediaFile::HasValidVideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::HasValidVideoAttributes(%d, %f, %f)\n", videoStatus, nMinDuration.GetSec(), nMaxDuration.GetSec());

	eVideoStatus nReturn = kVideoStatus_Success;

	// This still needs to use GetVideoContext because it does not use threads to call LoadFramePacket
	AVCodecContext* pVideoCodecContext = GetVideoContext(true);

	if (INVALID_PTR(pVideoCodecContext))
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad pointer found in video data structures.\n");

		if (nReturn == kVideoStatus_Success)
			nReturn = kVideoStatus_BadPointer;

		videoStatus.SetFail(kVideoStatus_BadPointer);
		return nReturn;  // Unlike other errors we have to return right away if this one fails or we will crash
	}
	else
	{
		videoStatus.SetPass(kVideoStatus_BadPointer);
	}

	switch (pVideoCodecContext->codec_id)
	{
		case AV_CODEC_ID_H264:
		{
			nReturn = _HasValidH264VideoAttributes(videoStatus, nMinDuration, nMaxDuration);
			break;
		}

		case AV_CODEC_ID_VP9:
		{
			nReturn = _HasValidVP9VideoAttributes(videoStatus, nMinDuration, nMaxDuration);
			break;
		}
		
		default:
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad codec id %i\n", pVideoCodecContext->codec_id);

			if (nReturn == kVideoStatus_Success)
			{
				nReturn = kVideoStatus_BadCodec;
			}

			videoStatus.SetFail(kVideoStatus_BadCodec);
			
			break;
		}
	}

	return nReturn;
}

//----------------------------------------------------------------------------------------------------------------------
// Validate the audio attributes
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eAudioStatus clMediaFile::HasValidAudioAttributes(stAudioStatus& audioStatus)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::HasValidAudioAttributes() %s\n", m_strFilename.c_str());
	
	eAudioStatus nReturn = kAudioStatus_Success;

	// Check all pointers that we're going to access below so we don't crash.  
	// This also validates that we have an audio stream.
	if (INVALID_PTR(m_pAudioCodecContext))
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad pointer found in video data structures.\n");

		if (nReturn == kAudioStatus_Success)
			nReturn = kAudioStatus_BadPointer;

		audioStatus.SetFail(kAudioStatus_BadPointer);
	}
	else
	{
		audioStatus.SetPass(kAudioStatus_BadPointer);
	}

	// Check that we have an audio stream
	if (!GetHasAudio())
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad audio stream, no audio.\n");

		if (nReturn == kAudioStatus_Success)
			nReturn = kAudioStatus_BadAudioStream;

		audioStatus.SetFail(kAudioStatus_BadAudioStream);
	}
	else
	{
		audioStatus.SetPass(kAudioStatus_BadAudioStream);	
	}


	// Check that codec_type is audio
	if (AVMEDIA_TYPE_AUDIO != m_pAudioCodecContext->codec_type)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad audio stream, codec type is: %i\n", m_pAudioCodecContext->codec_type);

		if (nReturn == kAudioStatus_Success)
			nReturn = kAudioStatus_BadType;

		audioStatus.SetFail(kAudioStatus_BadType);
	}
	else
	{
		audioStatus.SetPass(kAudioStatus_BadType);	
	}

	// Check for one of the codec types that we know doesn't work
	if ( AV_CODEC_ID_ADPCM_IMA_QT == m_pAudioCodecContext->codec_id )  // Bug 4427
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad audio codec id: %i\n", m_pAudioCodecContext->codec_id);

		if (nReturn == kAudioStatus_Success)
			nReturn = kAudioStatus_BadType;

		audioStatus.SetFail(kAudioStatus_BadType);
		return nReturn;
	}
	else
	{
		audioStatus.SetPass(kAudioStatus_BadType);
	}

	// NOTE:  Always do this test last because it can be slow and if there is something unforseen it could
	//        crash the decoder.  If we can fail for any other reason we should.
	// Test to see if we can get a good frame back in a reasonable amount of time.
	clAudioFrame testFrame;
	double decodeTime = getTimeMs();
	GetNextAudioFrame(testFrame);
	decodeTime = getTimeMs() - decodeTime;
	// NOTE:  We can't test result from GetNextAudioFrame or testFrame.isValid() 
	if (!testFrame.GetPassed() || decodeTime > 1000.0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Bad audio stream, failed to decode\n");

		if (nReturn == kAudioStatus_Success)
			nReturn = kAudioStatus_BadStream;

		audioStatus.SetFail(kAudioStatus_BadStream);
	}
	else
	{
		audioStatus.SetPass(kAudioStatus_BadStream);	
	}
	
	return nReturn;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetCurrentVideoTimestamp()
{
	return clTimeStamp(ConvertVideoPtsToMS(m_nCurrentPts));
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetCurrentAudioTimestamp()
{
	return clTimeStamp(m_bHasAudio ? ConvertAudioPtsToMS(m_nCurrentAudioPts) : 0.0);
}

//----------------------------------------------------------------------------------------------------------------------
// ctor
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::clMediaFile( bool useContextWrapper ) : 
	clMediaFileBase(),
	m_pFormatContext(nullptr),
	m_pVideoCodecContext(nullptr),
	m_pAudioCodecContext(nullptr),
	m_nVideoCodecId(AV_CODEC_ID_NONE),
	m_reOpenSeekTime(0.0),
	m_sumDecodeTime(0.0),
	m_minDecodeTime(10000000.0),
	m_maxDecodeTime(0.0),
	m_nDecodeEndTime(0.0),
	m_nFrameTimingCounter(0),

	m_pkt(),
	m_frame(nullptr),
	m_frameAudio(nullptr),

	m_SwrContext(nullptr),
	m_ppnAudioDstData(nullptr),

	m_nCurrentPts(0),
	m_nCurrentAudioPts(0),

	m_nLastSampleFreq(0),
	m_nVideoStreamIdx(-1),
	m_nAudioStreamIdx(-1),
	m_bResend(false),
	m_bUseContextWrapper(useContextWrapper),
	m_bGotKeyframe(true)
{
	m_audioDecodeMutex.Initialize();
	m_videoDecodeMutex.Initialize();
	memset(&m_pkt, 0x00, sizeof(m_pkt));
}

//----------------------------------------------------------------------------------------------------------------------
// xtor
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::~clMediaFile()
{
	if (m_bOpened)
	{
		Close();
	}

	m_audioDecodeMutex.Terminate();
	m_videoDecodeMutex.Terminate();
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::Close()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::Close(%s)\n", m_strFilename.c_str());

	// Some av_* functions are not thread safe
	nmThread::clCriticalSectionSimpleLock Lock(m_criticalSectionMediaFile);

	if (m_bUseContextWrapper)
	{
		s_videoContextWrapper.Close(this);
	}
	else
	{
		avcodec_free_context(&m_pVideoCodecContext);
	}

	avcodec_free_context(&m_pAudioCodecContext);

	avformat_close_input(&m_pFormatContext);

	if (m_ppnAudioDstData != nullptr && m_ppnAudioDstData[0] != nullptr)
	{
		av_freep(&m_ppnAudioDstData[0]);
	}

	if (m_ppnAudioDstData != nullptr)
	{
		av_free(m_ppnAudioDstData);
		m_ppnAudioDstData = nullptr;
	}
	
	if (m_SwrContext != nullptr)
	{
		swr_free(&m_SwrContext);
		m_SwrContext = nullptr;
	}

	av_packet_unref(&m_pkt);
	av_frame_unref(m_frame);
	av_frame_free(&m_frame);
	av_frame_unref(m_frameAudio);
	av_frame_free(&m_frameAudio);

	// Clear queued packets
	_ClearPacketQueues();

	// Clear these at the end, some functions above may check on them
	m_bOpened = false;
	m_bHasAudio = false;
	m_bHasVideo = false;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::clVideoFrame::eColorSpace ConvertColorSpace(AVColorSpace s)
{
	switch (s)
	{
	case AVCOL_SPC_UNSPECIFIED:
	case AVCOL_SPC_BT709:
	case AVCOL_SPC_BT470BG:   // TODO: This is coming from theme vids - have we handled this right?
	case AVCOL_SPC_SMPTE170M: // Pixel phone video has this color space
		return clMediaFile::clVideoFrame::k_ColorSpace_BT709;
		break;
	case AVCOL_SPC_BT2020_NCL:
	case AVCOL_SPC_BT2020_CL:
		return clMediaFile::clVideoFrame::k_ColorSpace_BT2020;
		break;
	default:
		NM_ASSERTMSG(false, "Unsupported color space: %i\n", s);
		return clMediaFile::clVideoFrame::k_ColorSpace_BT709;
		break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
std::string GetColorSpaceName(AVColorSpace s)
{
	switch (s)
	{
	case AVCOL_SPC_UNSPECIFIED:
		return "unspecified";
	case AVCOL_SPC_BT709:
		return "BT709";
	case AVCOL_SPC_BT470BG:   // TODO: This is coming from theme vids - have we handled this right?
		return "BT470BG interpreted as BT709";
	case AVCOL_SPC_SMPTE170M: // Stuart's Pixel phone video has this color space
		return "SMPTE170M interpreted as BT709";
	case AVCOL_SPC_BT2020_NCL:
		return "BT2020NCL intepreted as BT2020";
	case AVCOL_SPC_BT2020_CL:
		return "BT2020CL interpreted as BT2020";
	default:
		NM_ASSERTMSG(false, "Unsupported color space: %i\n", s);
		return "Unknown";
	}
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eOpenFileStatus clMediaFile::OpenFile(const char* pFilename, int streamFlags)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::OpenFile(%s, %d)\n", pFilename, streamFlags);

	eOpenFileStatus errorStatus = kOpenFileStatus_Success;

	do
	{
		// Some av_* functions (avcodec_open2) are not thread safe
		nmThread::clCriticalSectionSimpleLock Lock(m_criticalSectionMediaFile);

		if (avformat_open_input(&m_pFormatContext, pFilename, nullptr, nullptr) < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Could not open source file: %s\n", pFilename);
			errorStatus = kOpenFileStatus_NotFound;
			break;
		}

		// DTS = Decode Time Stamp:  May start negative, always monotonically increasing
		// PTS = Presentation Time Stamp:  Starts at 0, may not be in order if video has B-frames
		// The default seek and index file use DTS timing.  We will stick to this and convert our 
		// internal time requests by subtracting first_dts.
		//m_pFormatContext->iformat->flags |= AVFMT_SEEK_TO_PTS;  

#if defined(NM_DEBUG)
		m_pFormatContext->debug = FF_FDEBUG_TS;
#endif

		// retrieve stream information 
		// May have 1 or 2x pipelineSize frames decoded with this call 
		// so use this 1ms time limit to stop it.  This causes some videodec2
		// printed errors but no functional issues.  TODO: ffmpeg 
		m_pFormatContext->max_analyze_duration = 100;
		if (avformat_find_stream_info(m_pFormatContext, nullptr) < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Could not find stream information for file: %s\n", pFilename);
			errorStatus = kOpenFileStatus_NoStreamInfo;
			break;
		}
#if defined(NM_DEBUG)
		av_dump_format(m_pFormatContext, 0, pFilename, false);
#endif

		m_strFilename = std::string(pFilename);

		m_bHasVideo = (streamFlags & kVideoStream);

		if (m_bHasVideo)
		{
			eOpenFileStatus rVal = m_bUseContextWrapper ? s_videoContextWrapper.Open(this) : _OpenCodecContext(AVMEDIA_TYPE_VIDEO, m_nVideoStreamIdx, m_pVideoCodecContext);

			if (kOpenFileStatus_Success != rVal)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "Could not find video in the input, aborting\n");
				errorStatus = rVal;
				break;
			}
			else
			{
				//auto pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
				//float fps = float(pStream->avg_frame_rate.num) / float(pStream->avg_frame_rate.den);
				//NM_TRACESIMPLE("Opened video file: %s w: %i h: %i fps: %f color: %s\n", pFilename, m_pVideoCodecContext->width, m_pVideoCodecContext->height, fps, GetColorSpaceName(m_pVideoCodecContext->colorspace).c_str());
			}

			// This checks that the pixels are "square" and won't be stretched after they're decoded.  
			// We were going to error out and exit here but 1080p system videos are 136:135 to stretch 1088 into 1080 so 
			// we have to allow it 
			AVRational aspectRatio = m_pFormatContext->streams[m_nVideoStreamIdx]->sample_aspect_ratio;
			if (aspectRatio.num != 0 && aspectRatio.num != aspectRatio.den)
			{
				NM_TRACESIMPLE("Warning: Video %s has sample aspect ratio %i:%i\n", pFilename, aspectRatio.num, aspectRatio.den);
				FFMPEGUtils::Log(AV_LOG_WARNING, "Video %s has sample aspect ratio %i:%i\n", pFilename, aspectRatio.num, aspectRatio.den);
				//errorStatus = kOpenFileStatus_BadSampleAspectRatio;
				//break;
			}

			AVCodecContext* pVideoCodecContext = GetVideoContext(false);

			pVideoCodecContext->flags2 |= AV_CODEC_FLAG2_FAST;

			m_nRotationAngleDegrees = 0.f;
			AVStream* pVideoStream = m_pFormatContext->streams[m_nVideoStreamIdx];
			if (VALID_PTR(pVideoStream->metadata))
			{
				AVDictionaryEntry* avde = av_dict_get(pVideoStream->metadata, "rotate", nullptr, 0);
				if (VALID_PTR(avde))
				{
					m_nRotationAngleDegrees = atof(avde->value);
				}
			}

			m_nVideoCodecId = pVideoCodecContext->codec_id;
		}

		// Video files don't have to have audio
		bool bNeedsAudio = streamFlags == kAudioStream;
		m_bHasAudio = (streamFlags & kAudioStream);

		if (m_bHasAudio)
		{
			eOpenFileStatus rVal = _OpenCodecContext(AVMEDIA_TYPE_AUDIO, m_nAudioStreamIdx, m_pAudioCodecContext);
			if (kOpenFileStatus_Success != rVal)
			{
				if (bNeedsAudio)
				{
					FFMPEGUtils::Log(AV_LOG_ERROR, "Could not find audio in the input, aborting\n");
					errorStatus = rVal;
					break;
				}
				else
				{
					m_bHasAudio = false;
				}
			}
		}

		if (m_bHasAudio)
		{
			int nb_planes = av_sample_fmt_is_planar(m_pAudioCodecContext->sample_fmt) ? m_pAudioCodecContext->channels : 1;
			m_ppnAudioDstData = (uint8_t**)av_mallocz(sizeof(uint8_t *) * nb_planes);
			if (nullptr == m_ppnAudioDstData)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "Could not allocate audio data buffers\n");
				errorStatus = kOpenFileStatus_MemoryAllocFailed;
				break;
			}

			for (int i = 0; i < nb_planes; ++i)
			{
				m_ppnAudioDstData[i] = nullptr;
			}

			// Set up SWR context once you've got codec information
			m_SwrContext = swr_alloc();
			if (nullptr == m_SwrContext)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "Could not allocate swr context\n");
				errorStatus = kOpenFileStatus_MemoryAllocFailed;
				break;
			}

			m_nLastSampleFreq = m_pAudioCodecContext->sample_rate;

			// Bug Fix:  We have seen some videos that report planar data but have only one audio channel (logitech)
			// Without this fix either the video will get rejected or swr_convert will crash.
			// Note: I have seen videos where channels == 2 here and channels == 1 later after decode.  If I try
			// to fix these the same way in the decodeaudio function then the audio is messed up so those have to be rejected.
			if (m_pAudioCodecContext->channels == 1 && m_pAudioCodecContext->sample_fmt == AV_SAMPLE_FMT_FLTP)
			{
				m_pAudioCodecContext->sample_fmt = AV_SAMPLE_FMT_FLT;
			}

			// set input sample format
			av_opt_set_sample_fmt(m_SwrContext, "in_sample_fmt", m_pAudioCodecContext->sample_fmt, 0);

			// channel_layout is not always set (wav files for some reason?)
			m_pAudioCodecContext->channel_layout = av_get_default_channel_layout(m_pAudioCodecContext->channels);
			av_opt_set_int(m_SwrContext, "in_channel_layout", m_pAudioCodecContext->channel_layout, 0);

			// set input sample rate
			av_opt_set_int(m_SwrContext, "in_sample_rate", m_pAudioCodecContext->sample_rate, 0);

			// output channel layout is always stereo
			av_opt_set_int(m_SwrContext, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
			
			// output at the system sample rate
			av_opt_set_int(m_SwrContext, "out_sample_rate", m_audioParams.nSampleRate, 0);

			// output floats
			av_opt_set_sample_fmt(m_SwrContext, "out_sample_fmt", AV_SAMPLE_FMT_FLT, 0);

			int ret = swr_init(m_SwrContext);
			if (ret < 0)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "Could not initialize audio conversion context\n");
				errorStatus = kOpenFileStatus_CannotConvertAudioFormat;
				break;
			}
		}

		m_frame = av_frame_alloc();
		if (!m_frame)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Could not allocate frame\n");
			errorStatus = kOpenFileStatus_MemoryAllocFailed;
			break;
		}

		m_frameAudio = av_frame_alloc();
		if (!m_frameAudio)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Could not allocate audio frame\n");
			errorStatus = kOpenFileStatus_MemoryAllocFailed;
			break;
		}

		// initialize packet, set data to nullptr, let the demuxer fill it 
		av_init_packet(&m_pkt);
		m_pkt.data = nullptr;
		m_pkt.size = 0;

		m_bEOF = false;  // A Close() then Open() was leaving the EOF in a true state
		m_bFinishedVideo = false;
		m_bFinishedAudio = false;
		m_bEndVideo = false;
		m_bEndAudio = false;

		if (streamFlags == kAudioStream)  // This should be ==, not & because we want to use vid stream if present
		{
			do
			{
				// If the packet isn't freed we get a memory leak - fixed for patch 1.07 
				// Confirmed for video streams below, not sure about audio streams...
				av_packet_unref(&m_pkt);
				av_read_frame(m_pFormatContext, &m_pkt);
			} while (m_pkt.stream_index != m_nAudioStreamIdx);

			// NOTE:  If AVFMT_SEEK_TO_PTS is not enabled or not working, then we'd have to use
			// m_audioStream->first_dts instead of 0 to get to the beginning.
			_SeekFrame(m_nAudioStreamIdx, 0, 0);
		}
		else
		{
			// Replaced GetNextVideoFrame with simply reading the packet since 
			// the frame data itself is unused.  TODO: ffmpeg 
			do
			{
				// If the packet isn't freed we get a memory leak - fixed for patch 1.07 
				av_packet_unref(&m_pkt);
				av_read_frame(m_pFormatContext, &m_pkt);
			} while (m_pkt.stream_index != m_nVideoStreamIdx);

			// NOTE:  If AVFMT_SEEK_TO_PTS is not enabled or not working, then we'd have to use
			// pStream->first_dts instead of 0 to get to the beginning.
			AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
			_SeekFrame(m_nVideoStreamIdx, pStream->index_entries[0].timestamp, AVSEEK_FLAG_BACKWARD);
		}

		m_nFirstPts = m_pkt.pts;

		m_bOpened = true;

		return kOpenFileStatus_Success;
	}
	while (false);  // This is a run-once loop to replace a goto on error condition

	// If we got here there was an error condition, clean up memory and return
	Close();

	return errorStatus;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::PrintVideoFrameIndex()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::PrintVideoFrameIndex()\n");

	AVStream *st = m_pFormatContext->streams[m_nVideoStreamIdx];
	FFMPEGUtils::Log(AV_LOG_INFO, "Video Stream: first dts: %f\n", ConvertVideoPtsToMS(st->first_dts));
	for (int j = 0; j < st->nb_index_entries; ++j)
	{
		FFMPEGUtils::Log(AV_LOG_INFO, "Frame: %i, time: %f, keyframe: %i, minDist: %i\n", j, ConvertVideoPtsToMS(st->index_entries[j].timestamp),
			st->index_entries[j].flags, st->index_entries[j].min_distance);
	}
}

//----------------------------------------------------------------------------------------------------------------------
// Reads and submits audio data packets until a frame comes out or an error occurs
// Time scale compresses or expands the audio
// Returns:
// -1 on error
//  0 - no errors but we didn't get a frame (EOF usually)
//  1 - no errors and we got a frame 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetNextAudioFrame(clAudioFrame& frame, float nTimeScale)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetNextAudioFrame()\n");

	if (m_bFinishedAudio)
	{
		return -1;
	}

	//frame.clear();
	bool gotFrame = false;
	int tempCount = 0;

	// Modified loop to closely match video frame logic, old code is in non-h264 section below
	do
	{
		if (!m_bResend)
		{
			if (m_pkt.data != nullptr)
			{
				av_packet_unref(&m_pkt);
				m_pkt.data = nullptr;
				m_pkt.size = 0;
			}
		}

		if (!m_bResend)
		{
			// Get next au packet and check for end of file
			int rfVal = av_read_frame(m_pFormatContext, &m_pkt);
			// This might be EOF or some other error, either way, this makes sure
			// that we break out of the loop here.
			if (rfVal == AVERROR_EOF)
			{
				m_bEOF = true;
				// If we're at the end of the file, we need to send a few empty packets
				// to get the rest of the frames out of the pipeline
				m_pkt.stream_index = m_nAudioStreamIdx;
				//m_pkt.data = nullptr;
				m_pkt.size = 0;

				FFMPEGUtils::Log(AV_LOG_ERROR, "GetNextAudioFrame(%s): av_read_frame returned: AVERROR_EOF\n", GetFilename().c_str());
			}
			else if (rfVal < 0)
			{
				PrintAVError(rfVal, "audio read frame");
			}
		}

		m_bResend = false;

		if (m_pkt.stream_index == m_nAudioStreamIdx)
		{
			std::shared_ptr<uint8_t> temp;
			int64_t modifiedInputSampleFreq = int64_t(float(m_pAudioCodecContext->sample_rate) * nTimeScale + 0.5f);
			int decodedSize = 0;

			if (GetIsEOF())
			{
				// 4096 is value used in ffmpeg sample
				int64_t out_samples = av_rescale_rnd(4096, m_audioParams.nSampleRate, modifiedInputSampleFreq, AV_ROUND_UP);

				// Calculate the size of the buffer that was allocated to hold out_samples samples as 2 channel float
				int audioDstBufSize = av_samples_get_buffer_size(nullptr, 2, (int)out_samples, AV_SAMPLE_FMT_FLT, 1);
				temp = std::shared_ptr<uint8_t>((uint8_t*)av_malloc(audioDstBufSize), av_free);
				NM_ASSERT(temp);
				m_ppnAudioDstData[0] = temp.get();

				// NOTE:  This function may not return as many output samples as calculated above when resampling since resampling
				// might need future data.
				int64_t pts = swr_next_pts(m_SwrContext, INT64_MIN);
				pts /= m_audioParams.nSampleRate;
				int convertedSamples = swr_convert(m_SwrContext, m_ppnAudioDstData, (int)out_samples, nullptr, 0);
				NM_ASSERTMSG(convertedSamples >= 0, "Error converting audio format\n");
				m_ppnAudioDstData[0] = nullptr;

				// Calculate the exact size of the decoded audio.  May be less than audioDstBufSize!
				decodedSize = av_samples_get_buffer_size(nullptr, 2, convertedSamples, AV_SAMPLE_FMT_FLT, 1);
			}
			else
			{
				//NM_TRACESIMPLE("Sending packet %f\n", ConvertVideoPtsToMS(m_pkt.pts));
				int ret = avcodec_send_packet(m_pAudioCodecContext, &m_pkt);
				if (ret == AVERROR(EAGAIN))
				{
					//NM_TRACESIMPLE("EAGAIN from send packet\n");
					// This means the decoder is full.  Read a frame out then m_bResend the SAME packet next time
					ret = avcodec_receive_frame(m_pAudioCodecContext, m_frameAudio);
					if (ret >= 0)
					{
						// Got a frame, but need to m_bResend the same packet
						m_bResend = true;
						gotFrame = true;
					}
					else if (ret == AVERROR(EAGAIN))
					{
						// Both send packet and receive frame returned EAGAIN which shouldn't happen
						//NM_TRACESIMPLE("Error:  Send packet and receive frame both sent EAGAIN, API violation\n");
					}
					else if (ret == AVERROR_EOF)
					{
						//NM_TRACESIMPLE("Got EOF from receive frame\n");
						m_bFinishedAudio = true;
						_Flush();
					}
					else
					{
						PrintAVError(ret, "receive frame");
					}
				}
				else if (ret == AVERROR_EOF)
				{
					//NM_TRACESIMPLE("Got EOF from send packet\n");
				}
				else if (ret < 0)
				{
					PrintAVError(ret, "send packet");
				}

				if (!gotFrame)
				{
					// We didn't get a frame above, so try now
					ret = avcodec_receive_frame(m_pAudioCodecContext, m_frameAudio);
					if (ret >= 0)
					{
						gotFrame = true;
					}
					else if (ret == AVERROR(EAGAIN))
					{
						// Output is not available yet, send a NEW packet
						//NM_TRACESIMPLE("EAGAIN from receive frame\n");
					}
					else if (ret == AVERROR_EOF)
					{
						//NM_TRACESIMPLE("Got EOF from receive frame\n");
						m_bFinishedAudio = true;
						_Flush();
					}
					else
					{
						PrintAVError(ret, "receive frame");
					}
				}
				
				// Note:  Don't "else" this with the !gotFrame above, value can change
				if ( gotFrame )  
				{
					// Bug Fix DT 4513.  Audio format said planar, but it was mono
					if (av_sample_fmt_is_planar(m_pAudioCodecContext->sample_fmt) && INVALID_PTR(m_frameAudio->data[1]))
					{
						FFMPEGUtils::Log(AV_LOG_INFO, "Planar data expected but found only one channel after decoding\n");
						return -1;
					}

					/*FFMPEGUtils::Log(AV_LOG_INFO, "audio_frame%s n:%d nb_samples:%d pts:%s\n",
								 cached ? "(cached)" : "",
								 audio_frame_count++, frame->nb_samples,
								 av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));*/

								 // Don't change context parameters unless necessary since swr_init clears 
								 // the cached data related to resampling
					if (modifiedInputSampleFreq != m_nLastSampleFreq)
					{
						av_opt_set_int(m_SwrContext, "in_sample_rate", modifiedInputSampleFreq, 0);
						// Context must be re-initialized for changed parameter to take effect
						ret = swr_init(m_SwrContext);
						NM_ASSERTMSG(ret >= 0, "Could not initialize audio conversion context\n");
						m_nLastSampleFreq = modifiedInputSampleFreq;
						FFMPEGUtils::Log(AV_LOG_INFO, "Re-initializing m_SwrContext.\n");
					}
					// Calculate the maximum number of output samples
					// For some reason, adding delay causes buzzing in the audio during fast playback
					//int64_t delay = swr_get_delay(m_SwrContext, m_pAudioCodecContext->sample_rate);
					int64_t out_samples = av_rescale_rnd(/*delay + */ m_frameAudio->nb_samples,
						m_audioParams.nSampleRate, modifiedInputSampleFreq, AV_ROUND_UP);

					// Calculate the size of the buffer that was allocated to hold out_samples samples as 2 channel float
					int audioDstBufSize = av_samples_get_buffer_size(nullptr, 2, (int)out_samples, AV_SAMPLE_FMT_FLT, 1);
					temp = std::shared_ptr<uint8_t>((uint8_t*)av_malloc(audioDstBufSize), av_free);
					NM_ASSERT(temp);
					m_ppnAudioDstData[0] = temp.get();

					// The reason for this check of m_frame->nb_samples is that we were getting an occasional crash when
					// seeking.  The crash was inside the swr_convert call and somehow related to uncommon sample sizes (or
					// maybe changing sample sizes) when calling swr_convert.  The conversion was from signed 16 bit planar (mp3) to
					// interleaved float.  The crash was accessing a null pointer, most likely m_SwrContext->in->ch[1]
					// Probably responsible for DT3416, DT3418, and DT3497 
					// There is also a special case to let non-mp3 data pass through.  Packet sizes for those are less consistent.
					int convertedSamples = 0;
					if (m_frameAudio->nb_samples == 1024
						|| m_frameAudio->nb_samples == 1152
						|| (VALID_PTR(m_pAudioCodecContext) && AV_CODEC_ID_MP3 != m_pAudioCodecContext->codec_id)
						)
					{
						// NOTE:  This function may not return as many output samples as calculated above when resampling since resampling
						// might need future data.
						convertedSamples = swr_convert(m_SwrContext, m_ppnAudioDstData, (int)out_samples, (const uint8_t**)m_frameAudio->extended_data, m_frameAudio->nb_samples);
						m_ppnAudioDstData[0] = nullptr;

						if (convertedSamples > 0)
						{
							// Calculate the exact size of the decoded audio.  May be less than audioDstBufSize!
							decodedSize = av_samples_get_buffer_size(nullptr, 2, convertedSamples, AV_SAMPLE_FMT_FLT, 1);
							if (decodedSize < 0)
							{
								FFMPEGUtils::Log(AV_LOG_ERROR, "av_samples_get_buffer_size() Returned: %d\n", decodedSize);
							}
						}
						else
						{
							FFMPEGUtils::Log(AV_LOG_ERROR, "swr_convert() Returned: %d\n", convertedSamples);
						}

						//FFMPEGUtils::Log(AV_LOG_INFO, "I-samples: %i, scale: %f, o-samples: %i, c-samples: %i, dec size: %i, buf size: %i\n", 
						//	m_frameAudio->nb_samples, timeScale, out_samples, convertedSamples, decodedSize, audioDstBufSize); 
						//FFMPEGUtils::Log(AV_LOG_INFO, "Got audio packet, time = %i, pts  = %i, delay = %i\n", ConvertAudioPtsToMS(m_pkt.pts), m_pkt.pts, delay);
					}
					else
					{
						m_ppnAudioDstData[0] = nullptr;  // I believe forgetting this line was causing crashes in 1.08 when audio was being freed
						FFMPEGUtils::Log(AV_LOG_WARNING, "Skipped %i audio samples in %s to avoid ffmpeg swr_convert crash bug.\n", m_frameAudio->nb_samples, m_strFilename.c_str());
					}
				}
			}

			// For audio decode validation 
			frame.SetPassed(gotFrame || decodedSize > 0);

			if (decodedSize > 0)
			{
				gotFrame = true;
				m_nCurrentAudioPts = m_pkt.pts - m_nFirstPts;  // subtract first frame pts so it starts at 0
				frame.SetBuffer(temp);
				frame.SetSize(decodedSize);
				frame.SetPts(m_nCurrentAudioPts, GetAudioPtsToMS());
				//NM_TRACESIMPLE("Got audio frame %i %f\n", m_pkt.pts, outFrame.GetTimeStamp().GetMs());
			}
			else if (GetIsEOF())
			{
				// Nothing decoded and EOF means we're done
				m_bFinishedAudio = true;
			}

			++tempCount;
		}
		// If we're not at the end of the file, repeat until we get a frame back
	} while (m_pkt.stream_index != m_nAudioStreamIdx || (gotFrame == false && !m_bEOF));

	int ret = gotFrame ? 1 : (m_bEOF ? -1 : 0);

	if (ret > 0)
	{
		if (m_pkt.stream_index == m_nAudioStreamIdx)
		{
			av_packet_unref(&m_pkt);
			//FFMPEGUtils::Log(AV_LOG_INFO, "---------Allocated Audio Frame %x with pts %d\n", outFrame.buffer,outFrame.pts);
		}
	}

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------
//  Returns number of packets loaded
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::LoadFramePacket(const clTimeStamp& clipEnd, bool bNeedsAudio)
{
	// Update active video codec
	if (m_bUseContextWrapper)
	{
		AVCodecContext* pVideoCodecContext = s_videoContextWrapper.GetVideoContext(this, true);
		if (pVideoCodecContext != m_pVideoCodecContext)
		{
			m_videoDecodeMutex.Lock();
			m_pVideoCodecContext = pVideoCodecContext;
			m_videoDecodeMutex.Unlock();
		}
	}

	// File is done, nothing more to load
	if (m_bEOF)
	{
		return 0;
	}

	// Queues are full, consume some first then try again later
	if ( m_qVideoPacket.Size() >= GetMaxVideoPacketsToBuffer()
	  || m_qAudioPacket.Size() >= GetMaxAudioPacketsToBuffer())
	{
		return 0;
	}

	// Must be freed later with av_packet_free()
	AVPacket* pPacket = av_packet_alloc();
	NM_ASSERTPTR(pPacket);
	av_init_packet(pPacket);

	// Get next packet.  We do not need to wrap this in a mutex because the load thread is the only
	// one that can do seeking.
	int rfVal = av_read_frame(m_pFormatContext, pPacket);
	
	// Check error values
	if ( rfVal == AVERROR_EOF )
	{
		//NM_TRACESIMPLE("Got EOF from read frame\n");
		m_bEOF = true;

		// At the end of the file, we need to send an empty packet to both streams to put them into draining mode
		//NM_TRACESIMPLE("Cancelling load due to end of file\n");
		av_packet_unref(pPacket);
		pPacket->stream_index = m_nVideoStreamIdx;
		pPacket->data = nullptr;
		pPacket->size = 0;
		m_bEndVideo = true;
		m_qVideoPacket.Put(pPacket);

		if (GetHasAudio() && bNeedsAudio)
		{
			AVPacket* pAudioPacket = av_packet_alloc();
			NM_ASSERTPTR(pPacket);
			av_init_packet(pAudioPacket);
			pAudioPacket->stream_index = m_nAudioStreamIdx;
			pAudioPacket->data = nullptr;
			pAudioPacket->size = 0;
			m_bEndAudio = true;
			m_qAudioPacket.Put(pAudioPacket);
		}

		FFMPEGUtils::Log(AV_LOG_ERROR, "LoadFramePacket(%s): av_read_frame returned: AVERROR_EOF\n", GetFilename().c_str());
		return 1;
	}
	else if (rfVal < 0)
	{
		PrintAVError(rfVal, "Load Frame Packet");
		//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
		av_packet_free(&pPacket);
		return 0;
	}

	const clTimeStamp slop(300.0);

	if (pPacket->stream_index == m_nVideoStreamIdx)
	{
		if (!m_bHasVideo || m_bEndVideo)
		{
			//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
			av_packet_free(&pPacket);
			return 0;
		}

		// Check to see if we're past the trim point in this clip
		clTimeStamp packetTime;
		packetTime.SetPts(pPacket->pts - m_nFirstPts, GetVideoPtsToMS());
		if (packetTime > clipEnd + slop)
		{
			// NM_TRACESIMPLE("Cancelling load after video Packet: %f, end: %f stream %i\n", packetTime.GetMs(), clipEnd.GetMs(), pPacket->stream_index);
			av_packet_unref(pPacket);
			pPacket->stream_index = m_nVideoStreamIdx;
			pPacket->data = nullptr;
			pPacket->size = 0;
			m_bEndVideo = true;
		}
		// NM_TRACESIMPLE("Loaded packet %f, key = %i\n", packetTime.GetSec(), pPacket->flags);
		++m_nKeyframeIntervalCounter;
		if (pPacket->flags & AV_PKT_FLAG_KEY)
		{
			m_nKeyframeInterval = m_nKeyframeIntervalCounter;
			m_nKeyframeIntervalCounter = 0;

			// If we close then re-open the shared video context, come back to this point
			m_reOpenSeekTime.SetPts(pPacket->pts, GetVideoPtsToMS());
		}

		m_qVideoPacket.Put(pPacket);

		return 1;
	}
	else if (pPacket->stream_index == m_nAudioStreamIdx)
	{
		if (!m_bHasAudio || !bNeedsAudio || m_bEndAudio)
		{
			//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
			av_packet_free(&pPacket);
			return 0;
		}

		// Check to see if we're past the trim point in this clip
		clTimeStamp packetTime;
		packetTime.SetPts(pPacket->pts - m_nFirstPts, GetAudioPtsToMS());
		if (packetTime > clipEnd + slop)
		{
			// NM_TRACESIMPLE("Cancelling load after audio Packet: %f, end: %f stream %i\n", packetTime.GetMs(), clipEnd.GetMs(), pPacket->stream_index);
			av_packet_unref(pPacket);
			pPacket->stream_index = m_nAudioStreamIdx;
			pPacket->data = nullptr;
			pPacket->size = 0;
			m_bEndAudio = true;
		}
		m_qAudioPacket.Put(pPacket);
		return 1;
	}
	else
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Unknown stream in media file\n");
		//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
		av_packet_free(&pPacket);
		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Get a frame from the given context, can be audio or video
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::_ReceiveFrame(AVCodecContext *pContext, AVFrame *frame, bool& gotFrame, bool& finished)
{
	int ret = avcodec_receive_frame(pContext, frame);
	if (ret >= 0)
	{
		gotFrame = true;
	}
	else if (ret == AVERROR(EAGAIN))
	{
		// Need to call receive_frame again later. 
		// If this comes after a call to send frame, it's an API violation error
		// If it comes after a call to receive frame, it means the decoder needs another send frame first
		//NM_TRACESIMPLE("Got EAGAIN from receive frame\n");
	}
	else if (ret == AVERROR_EOF)
	{
		// Decoder has been fully flushed and there will be no more output frames
		//NM_TRACESIMPLE("Got EOF from receive frame\n");
		finished = true;
		_Flush(pContext);
	}
	else
	{
		PrintAVError(ret, "receive frame");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Returns fist keyframe present in m_qVideoPacket or nullptr if not found
//----------------------------------------------------------------------------------------------------------------------
AVPacket* clMediaFile::SkipToFirstKeyframePacketVideo()
{
	// See if there is a keyframe in the packet list
	class CountBeforeKeyframe : public nmSystem::clFifoVisitor<AVPacket*>
	{
	public:
		virtual bool operator () (AVPacket*& item)
		{
			++count;
			return !(item->flags & AV_PKT_FLAG_KEY);
		}

		int count = 0;
	};
	CountBeforeKeyframe counter;
	m_qVideoPacket.Query(counter);

	AVPacket* pPacket = nullptr;
	if (m_qVideoPacket.Size() != counter.count)
	{
		// There is a keyframe in the queue, delete all packets prior to that keyframe.
		class DeleteUpToKeyframe : public nmSystem::clFifoVisitor<AVPacket*>
		{
		public:
			virtual bool operator () (AVPacket*& pPacket)
			{
				foundFirstKey |= (pPacket->flags & AV_PKT_FLAG_KEY);
				if (!foundFirstKey)
				{
					//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
					av_packet_free(&pPacket);
					return true;  // delete it from the list
				}

				return false;
			}

			bool foundFirstKey = false;
		};
		DeleteUpToKeyframe deleteUpToKeyframe;
		m_qVideoPacket.RemoveOnMatch(deleteUpToKeyframe);

		// Finally, pull out the keyframe for decoding
		m_qVideoPacket.Front(pPacket);
	}

	return pPacket;
}

//----------------------------------------------------------------------------------------------------------------------
//  Decode video packets until we get a frame or an error occurs
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::DecodeVideoPacket(std::shared_ptr<clVideoFrame>& pFrameOut, stFramePacker& fp, const clTimeStamp& beginningCutoffTime, bool keyframeOnly )
{
	if (m_bFinishedVideo)
	{
		return -1;
	}

	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::DecodeVideoPacket()\n");

	int tempCount = 0;
	bool gotFrame = false;
   
	AVFrame* pFrameAv = fp.m_pFrame == nullptr ? m_frame : fp.m_pFrame;

	nmThread::clScopedMutex scopedLock(&m_videoDecodeMutex);
	// Modified loop for pipelining and to return a frame if at all possible
	do
	{
		// Break early before doing any decoding - Patch 3.03 
		if (m_bShortCircuitDecoding)
		{
			m_bFinishedVideo = true;
			break;
		}

		// This used to call GetVideoContext(), but now m_pVideoCodecContext is updated in LoadFramePacket 
		// to make thread safety easier.  
		AVCodecContext* pVideoCodecContext = m_pVideoCodecContext;

		// It's possible for this to be null when doing something like:
		// stopDecoding, seek/reset, startDecoding if this function gets called before the LoadFramePacket function.
		if (nullptr == pVideoCodecContext)
		{
			break;
		}

		// Get the next packet 
		AVPacket* pPacket = nullptr;
		// If we're in keyframe only mode and we got a keyframe out last time, skip to the next keyframe
		if (keyframeOnly && m_bGotKeyframe)
		{
			pPacket = SkipToFirstKeyframePacketVideo();
			// If there is no next keyframe....
			if (nullptr == pPacket)
			{
				// If we're at end of file, there will never be another keyframe
				if (m_bEOF)
				{
					m_bFinishedVideo = true;
				}
				// No keyframe found, come back later and check again. 
				return 0;
			}
			else
			{
				// ok, we got a packet, that packet is a keyframe, now we decode normally till that keyframe pops out
				m_bGotKeyframe = false;
			}
		}
		else
		{
			m_qVideoPacket.Front(pPacket);
		}

		// If we got a packet, send it to the decoder
		if (nullptr != pPacket)
		{
			clTimeStamp ts;
			ts.SetPts(pPacket->pts, GetVideoPtsToMS());
			//NM_TRACESIMPLE("Submitting packet %f key %i mode: %i\n", ts.GetSec(), (pPacket->flags & AV_PKT_FLAG_KEY), keyframeOnly);
			int ret = avcodec_send_packet(pVideoCodecContext, pPacket);
			if (ret == AVERROR(EAGAIN))
			{
				// This means the decoder is full.  Read a frame out and send the same packet the next time
				_ReceiveFrame(pVideoCodecContext, pFrameAv, gotFrame, m_bFinishedVideo);
				//NM_TRACESIMPLE("Got EAGAIN from send packet\n");
			}
			else if (ret == AVERROR_EOF)
			{
				// This means the decoder has been flushed or more than one flush packet has been sent
				// We're done with that packet, discard it
				//NM_TRACESIMPLE("Got EOF from send packet\n");
				m_qVideoPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
			else if (ret < 0)
			{
				PrintAVError(ret, "send video packet");
				// We're done with that packet, discard it
				m_qVideoPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
			else
			{
				//NM_TRACESIMPLE("Got ok from send packet\n");
				// We're done with that packet, discard it
				m_qVideoPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
		}

		// If we didn't get a frame out above, do it now
		if (!gotFrame)
		{
			_ReceiveFrame(pVideoCodecContext, pFrameAv, gotFrame, m_bFinishedVideo);
		}

		// Note:  Don't "else" this with the !gotFrame above, value can change
		if (gotFrame)
		{
			m_bGotKeyframe = true;

			// Subtract first pts from actual pts we get because some decoding logic assumes
			// videos start at 0 but we have some from PPR that do not 
			pFrameOut->SetPts(pFrameAv->pts - m_nFirstPts, GetVideoPtsToMS());

			// Skip this frame if it's before clip start.  This happens because seeks go to keyframes that may precede what we care about
			if (pFrameOut->GetTimeStamp() >= beginningCutoffTime)
			{
				// Begin hack - PS5 system created videos report 1088 height but the data only goes to 1080
				int cropHeight = pVideoCodecContext->height;
				if (pVideoCodecContext->codec_id == AV_CODEC_ID_VP9 && pFrameAv->width == 1920 && pFrameAv->height == 1088)
				{
					pFrameAv->height = 1080;
					cropHeight = 1080;
				}
				// end hack

				//NM_TRACESIMPLE("Got video frame %f  start: %f corrected: %f\n", 0.001*ConvertVideoPtsToMS(pFrameAv->pts), GetFirstFrameVideoTimestamp().GetSec(), pFrameOut->GetTimeStamp().GetSec());
				pFrameOut->SetWidth(pFrameAv->width);
				pFrameOut->SetHeight(pFrameAv->height);
				pFrameOut->SetCropWidth(pVideoCodecContext->width);
				pFrameOut->SetCropHeight(cropHeight);
				pFrameOut->SetRotationDegrees(GetRotationAngleDegrees());
				pFrameOut->SetProgressive(pFrameAv->interlaced_frame == 0);
				int w = std::max(pFrameAv->width, pFrameAv->linesize[0]);
				pFrameOut->SetSize(av_image_get_buffer_size(pVideoCodecContext->pix_fmt, w, pFrameOut->GetHeight(), 1));
				pFrameOut->SetPacketSize(pFrameAv->pkt_size);
				pFrameOut->SetKeyframe(pFrameAv->key_frame != 0);
				pFrameOut->SetColorSpace(ConvertColorSpace(pFrameAv->colorspace));
				pFrameOut->CopyDecodeData(pFrameAv->data[0], pFrameAv->data[1], pFrameAv->data[2], pFrameAv->linesize[0], pFrameAv->linesize[1], pFrameAv->linesize[2], fp);
				m_nCurrentPts = pFrameAv->pts - m_nFirstPts;
			}
			else
			{
				gotFrame = false;
			}
		}

		if (++tempCount > GetDecoderPipelineDepth())
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "no frame returned from avcodec_decode_video2 after %i packets\n", tempCount);
			// Note related to Bug DT4783.  It does not seem possible to detect interlace video via avCodecContext.
			// On one particular video source, libVideoDec2 does not correctly set this flag for the decoded frame either.
			// That same video requires something like 19 packets input before the first frame is output.
			// Normally we should only need pipelineDepth worth but we have one 360 video that works
			// ok after 7 packets, so this value is set to pass the 360 video but not the interlace video.  May need
			// future tweaking.
			if (tempCount > 7)
			{
				pFrameOut->SetProgressive(false);  // It's not really progressive but this indicates unknown failure.  TODO Rename
				break;
			}
		}
	} while (gotFrame == false && !m_bEOF && !m_qVideoPacket.Empty());  // Repeat until we get a frame or reach end of file

	int ret = gotFrame ? 1 : (m_bEOF ? -1 : 0);

	if (ret > 0 && !pFrameOut->IsValid())
	{
		pFrameOut->SetTimeStamp(clTimeStamp::s_zero);
		FFMPEGUtils::Log(AV_LOG_ERROR, "invalid pFrameOut - frame pool may be too small.\n");
	}

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------
//  Decode audio packets until we get a frame or an error occurs
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::DecodeAudioPacket( clAudioFrame& frame, float nTimeScale, const clTimeStamp& clipStart)
{
	if ( m_bFinishedAudio )
	{
		return -1;
	}

	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetNextAudioFrame()\n");

	int tempCount = 0;
	bool gotFrame = false;

	nmThread::clScopedMutex scopedLock(&m_audioDecodeMutex);
	// Modified loop to closely match video frame logic, old code is in non-h264 section below
	do
	{
		// Break early before doing any decoding - Patch 3.03 
		if (m_bShortCircuitDecoding)
		{
			m_bFinishedAudio = true;
			break;
		}

		// Get the next packet
		AVPacket* pPacket = nullptr;
		if (m_qAudioPacket.Front(pPacket))
		{
			int ret = avcodec_send_packet(m_pAudioCodecContext, pPacket);
			if (ret == AVERROR(EAGAIN))
			{
				_ReceiveFrame(m_pAudioCodecContext, m_frameAudio, gotFrame, m_bFinishedAudio);
			}
			else if (ret == AVERROR_EOF)
			{
				// We're done with that packet, discard it
				m_qAudioPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
			else if (ret < 0)
			{
				PrintAVError(ret, "send audio packet");
				// We're done with that packet, discard it
				m_qAudioPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
			else
			{
				// We're done with that packet, discard it
				m_qAudioPacket.Pop();
				//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
				av_packet_free(&pPacket);
			}
		}

		if (!gotFrame)
		{
			_ReceiveFrame(m_pAudioCodecContext, m_frameAudio, gotFrame, m_bFinishedAudio);
		}

		// Note:  Don't "else" this with the !gotFrame above, value can change
		if (gotFrame)
		{
			std::shared_ptr<uint8_t> temp;
			int64_t modifiedInputSampleFreq = int64_t(float(m_pAudioCodecContext->sample_rate) * nTimeScale + 0.5f);
			int decodedSize = 0;

			// Bug Fix DT 4513.  Audio format said planar, but it was mono
			if (av_sample_fmt_is_planar(m_pAudioCodecContext->sample_fmt) && INVALID_PTR(m_frameAudio->data[1]))
			{
				FFMPEGUtils::Log(AV_LOG_INFO, "Planar data expected but found only one channel after decoding\n");
				return -1;
			}

			/*FFMPEGUtils::Log(AV_LOG_INFO, "audio_frame%s n:%d nb_samples:%d pts:%s\n",
						 cached ? "(cached)" : "",
						 audio_frame_count++, frame->nb_samples,
						 av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));*/

						 // Don't change context parameters unless necessary since swr_init clears
						 // the cached data related to resampling
			if (modifiedInputSampleFreq != m_nLastSampleFreq)
			{
				av_opt_set_int(m_SwrContext, "in_sample_rate", modifiedInputSampleFreq, 0);
				// Context must be re-initialized for changed parameter to take effect
				int ret = swr_init(m_SwrContext);
				NM_ASSERTMSG(ret >= 0, "Could not initialize audio conversion context\n");
				m_nLastSampleFreq = modifiedInputSampleFreq;
				FFMPEGUtils::Log(AV_LOG_INFO, "Re-initializing m_SwrContext.\n");
			}
			// Calculate the maximum number of output samples
			// For some reason, adding delay causes buzzing in the audio during fast playback
			// int64_t delay = swr_get_delay(m_SwrContext, m_pAudioCodecContext->sample_rate);
			int64_t out_samples = av_rescale_rnd(/*delay + */ m_frameAudio->nb_samples, m_audioParams.nSampleRate, modifiedInputSampleFreq, AV_ROUND_UP);

			// Calculate the size of the buffer that was allocated to hold out_samples samples as 2 channel float
			int audioDstBufSize = av_samples_get_buffer_size(nullptr, 2, (int)out_samples, AV_SAMPLE_FMT_FLT, 1);
			temp = std::shared_ptr<uint8_t>((uint8_t*)av_malloc(audioDstBufSize), av_free);
			NM_ASSERT(temp);
			m_ppnAudioDstData[0] = temp.get();

			// The reason for this check of m_frame->nb_samples is that we were getting an occasional crash when
			// seeking.  The crash was inside the swr_convert call and somehow related to uncommon sample sizes (or
			// maybe changing sample sizes) when calling swr_convert.  The conversion was from signed 16 bit planar (mp3) to
			// interleaved float.  The crash was accessing a null pointer, most likely m_SwrContext->in->ch[1]
			// Probably responsible for DT3416, DT3418, and DT3497  
			// There is also a special case to let non-mp3 data pass through.  Packet sizes for those are less consistent.
			int convertedSamples = 0;
			if (m_frameAudio->nb_samples == 1024 || m_frameAudio->nb_samples == 1152 ||
				(VALID_PTR(m_pAudioCodecContext) && AV_CODEC_ID_MP3 != m_pAudioCodecContext->codec_id))
			{
				// NOTE:  This function may not return as many output samples as calculated above when resampling since resampling
				// might need future data.
				convertedSamples = swr_convert(m_SwrContext, m_ppnAudioDstData, (int)out_samples, (const uint8_t**)m_frameAudio->extended_data, m_frameAudio->nb_samples);
				m_ppnAudioDstData[0] = nullptr;

				if (convertedSamples > 0)
				{
					// Calculate the exact size of the decoded audio.  May be less than audioDstBufSize!
					decodedSize = av_samples_get_buffer_size(nullptr, 2, convertedSamples, AV_SAMPLE_FMT_FLT, 1);
					if (decodedSize < 0)
					{
						FFMPEGUtils::Log(AV_LOG_ERROR, "av_samples_get_buffer_size() Returned: %d\n", decodedSize);
					}
				}
				else
				{
					FFMPEGUtils::Log(AV_LOG_ERROR, "swr_convert() Returned: %d\n", convertedSamples);
				}

				// FFMPEGUtils::Log(AV_LOG_INFO, "I-samples: %i, scale: %f, o-samples: %i, c-samples: %i, dec size: %i, buf size: %i\n",
				//	m_frameAudio->nb_samples, timeScale, out_samples, convertedSamples, decodedSize, audioDstBufSize);
				// FFMPEGUtils::Log(AV_LOG_INFO, "Got audio packet, time = %i, pts  = %i, delay = %i\n", ConvertAudioPtsToMS(pPacket->pts), pPacket->pts, delay);
			}
			else
			{
				m_ppnAudioDstData[0] = nullptr;	// I believe forgetting this line was causing crashes in 1.08 when audio was being freed
				FFMPEGUtils::Log(AV_LOG_WARNING, "Skipped %i audio samples in %s to avoid ffmpeg swr_convert crash bug.\n", m_frameAudio->nb_samples,
					m_strFilename.c_str());
			}

			// For audio decode validation
			m_nCurrentAudioPts = m_frameAudio->pts - m_nFirstPts;  // subtract first frame pts so it starts at 0
			frame.SetPts(m_nCurrentAudioPts, GetAudioPtsToMS());
			if ( frame.GetTimeStamp() >= clipStart )
			{
				frame.SetPassed(true);
				frame.SetBuffer(temp);
				frame.SetSize(decodedSize);
			}
			else
			{
				gotFrame = false;
			}
			//NM_TRACESIMPLE("Got audio frame %i %f\n", m_frameAudio->pts, frame.GetTimeStamp().GetMs());
		}
		else
		{
			frame.SetPassed(false);
		}

		++tempCount;
		// If we're not at the end of the file, repeat until we get a frame back
	} while (gotFrame == false && !m_bEOF && !m_qAudioPacket.Empty());

	int ret = gotFrame ? 1 : (m_bEOF ? -1 : 0);

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::_ClearPacketQueues()
{
	class PacketDelete : public nmSystem::clFifoVisitor<AVPacket*>
	{
	public:

		virtual bool operator () (AVPacket*& pPacket)
		{
			//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
			av_packet_free(&pPacket);
			return true;  // delete it from the list
		}
	};

	//NM_TRACESIMPLE("Clearing packet queues: v: %i a: %i\n", m_qVideoPacket.Size(), m_qAudioPacket.Size());

	PacketDelete pd;
	m_qVideoPacket.RemoveOnMatch(pd);
	m_qAudioPacket.RemoveOnMatch(pd);
}


//----------------------------------------------------------------------------------------------------------------------
// Reads and submits video data packets until a frame comes out or an error occurs
// Returns:
// -1 on error
//  0 - no errors but we didn't get a frame (EOF usually)
//  1 - no errors and we got a frame 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetNextVideoFrame(std::shared_ptr<clVideoFrame>& pFrame)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetNextVideoFrame()\n");

	if (m_bFinishedVideo)
	{
		return -1;
	}

	int tempCount = 0;
	bool gotFrame = false;
	bool fishyVideo = false;

	// Modified loop for pipelining and to return a frame if at all possible
	do
	{
		if (!m_bResend)
		{
			if (m_pkt.data != nullptr)
			{
				av_packet_unref(&m_pkt);
				m_pkt.data = nullptr;
				m_pkt.size = 0;
			}
		}

		// Break early before doing any decoding - Patch 3.03 
		if (m_bShortCircuitDecoding)
			break;

		if (!m_bResend)
		{
			// Get next au packet and check for end of file
			int rfVal = av_read_frame(m_pFormatContext, &m_pkt);
			//if ( m_pkt.stream_index == m_nVideoStreamIdx ) FFMPEGUtils::Log(AV_LOG_INFO, "Read frame packet %f\n", ConvertVideoPtsToMS(m_pkt.pts));

			if (rfVal == AVERROR_EOF)
			{
				//NM_TRACESIMPLE("Got EOF from read frame\n");

				m_bEOF = true;
				// If we're at the end of the file, we need to send an empty packet to put the stream into draining mode
				m_pkt.stream_index = m_nVideoStreamIdx;
				m_pkt.data = nullptr;
				m_pkt.size = 0;

				FFMPEGUtils::Log(AV_LOG_ERROR, "GetNextVideoFrameInternal(%s): av_read_frame returned: AVERROR_EOF\n", GetFilename().c_str());
			}
			else if (rfVal < 0)
			{
				PrintAVError(rfVal, "read frame");
			}
			//FFMPEGUtils::Log(AV_LOG_INFO, "Decode PTS time: %i, Time: %i, stream: %i\n", m_pkt.pts, ConvertVideoPtsToMS(m_pkt.pts), m_pkt.stream_index);
		}

		m_bResend = false;

		if (m_pkt.stream_index == m_nVideoStreamIdx)
		{
			AVCodecContext* pVideoCodecContext = GetVideoContext(true);

			//NM_TRACESIMPLE("Sending packet %f\n", ConvertVideoPtsToMS(m_pkt.pts));
			int ret = avcodec_send_packet(pVideoCodecContext, &m_pkt);
			if (ret == AVERROR(EAGAIN))
			{
				//NM_TRACESIMPLE("EAGAIN from send packet\n");
				// This means the decoder is full.  Read a frame out then m_bResend the SAME packet next time
				ret = avcodec_receive_frame(pVideoCodecContext, m_frame);
				if (ret >= 0)
				{
					// Got a frame, but need to m_bResend the same packet
					m_bResend = true;
					gotFrame = true;
				}
				else if (ret == AVERROR(EAGAIN))
				{
					// Both send packet and receive frame returned EAGAIN which shouldn't happen
					//NM_TRACESIMPLE("Error:  Send packet and receive frame both sent EAGAIN, API violation\n");
				}
				else if (ret == AVERROR_EOF)
				{
					//NM_TRACESIMPLE("Got EOF from receive frame\n");
					m_bFinishedVideo = true;
					_Flush();
				}
				else
				{
					PrintAVError(ret, "receive frame");
				}
			}
			else if (ret == AVERROR_EOF)
			{
				// This means the decoder has been flushed or more than one flush packet has been sent
				//NM_TRACESIMPLE("Got EOF from send packet\n");
			}
			else if ( ret < 0 )
			{
				PrintAVError(ret, "send packet");
			}

			if (!gotFrame)
			{
				// We didn't get a frame above, so try now
				ret = avcodec_receive_frame(pVideoCodecContext, m_frame);
				if (ret >= 0)
				{
					gotFrame = true;
				}
				else if (ret == AVERROR(EAGAIN))
				{
					// Output is not available yet, send a NEW packet
					//NM_TRACESIMPLE("EAGAIN from receive frame\n");
				}
				else if (ret == AVERROR_EOF)
				{
					//NM_TRACESIMPLE("Got EOF from receive frame\n");
					m_bFinishedVideo = true;
					_Flush(); 
				}
				else
				{
					PrintAVError(ret, "receive frame");
				}
			}

			// Note:  Don't "else" this with the !gotFrame above, value can change
			if (gotFrame)
			{
				// Begin hack - PS5 system created videos report 1088 height but the data only goes to 1080
				int cropHeight = pVideoCodecContext->height;
				if (pVideoCodecContext->codec_id == AV_CODEC_ID_VP9 && m_frame->width == 1920 && m_frame->height == 1088)
				{
					m_frame->height = 1080;
					cropHeight = 1080;
				}
				// end hack

				// Subtract first pts from actual pts we get because some decoding logic assumes
				// videos start at 0 but we have some from PPR that do not 
				pFrame->SetPts(m_frame->pts - m_nFirstPts, GetVideoPtsToMS());
				//NM_TRACESIMPLE("Got frame %f  start: %f corrected: %f\n", ConvertVideoPtsToMS(m_frame->pts), GetFirstFrameVideoTimestamp().GetMs(), pFrame->GetTimeStamp().GetMs());
				pFrame->SetWidth(m_frame->width);
				pFrame->SetHeight(m_frame->height);
				pFrame->SetCropWidth(pVideoCodecContext->width);
				pFrame->SetCropHeight(cropHeight);
				pFrame->SetRotationDegrees(GetRotationAngleDegrees());
				pFrame->SetProgressive(m_frame->interlaced_frame == 0);
				int w = std::max(m_frame->width, m_frame->linesize[0]);
				pFrame->SetSize(av_image_get_buffer_size(pVideoCodecContext->pix_fmt, w, pFrame->GetHeight(), 1));
				pFrame->SetPacketSize(m_frame->pkt_size);
				pFrame->SetKeyframe(m_frame->key_frame != 0);
				pFrame->SetColorSpace(ConvertColorSpace(m_frame->colorspace));
				stFramePacker fpDummy;  // not used
				pFrame->CopyDecodeData(m_frame->data[0], m_frame->data[1], m_frame->data[2], m_frame->linesize[0], m_frame->linesize[1], m_frame->linesize[2], fpDummy);
				m_nCurrentPts = m_frame->pts - m_nFirstPts;
			}

			if (++tempCount > GetDecoderPipelineDepth())
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "no frame returned from avcodec_decode_video2 after %i packets\n", tempCount);
				// Note related to Bug DT4783.  It does not seem possible to detect interlace video via avCodecContext.  
				// On one particular video source, libVideoDec2 does not correctly set this flag for the decoded frame either.
				// That same video requires something like 19 packets input before the first frame is output. 
				// Normally we should only need pipelineDepth worth but we have one 360 video that works 
				// ok after 7 packets, so this value is set to pass the 360 video but not the interlace video.  May need
				// future tweaking.
				if (tempCount > 7)
				{
					fishyVideo = true;
					break;
				}
			}
		}

		// If we're not at the end of the file, repeat until we get a frame back
	} while (m_pkt.stream_index != m_nVideoStreamIdx || (gotFrame == false && !m_bEOF));

	int ret = gotFrame ? 1 : (m_bEOF ? -1 : 0);

	if (fishyVideo)
	{
		pFrame->SetProgressive(false);
	}

	if (ret > 0 && m_pkt.stream_index == m_nVideoStreamIdx)
	{
		if (!pFrame->IsValid())
		{
			pFrame->SetTimeStamp(clTimeStamp::s_zero);
			FFMPEGUtils::Log(AV_LOG_ERROR, "invalid pFrame - frame pool may be too small.\n");
		}
	}

	return ret;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int64_t clMediaFile::ConvertSecondsToVideoPts(double seekPosInSeconds ) const
{
	int64_t tsms = (int64_t)(seekPosInSeconds * 1000.0f);
	return ConvertMSToVideoPts(tsms);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int64_t clMediaFile::ConvertMSToVideoPts(int64_t tsms) const
{
	int64_t desiredFrameNumber = av_rescale(tsms, m_pFormatContext->streams[m_nVideoStreamIdx]->time_base.den, m_pFormatContext->streams[m_nVideoStreamIdx]->time_base.num);
	desiredFrameNumber /= 1000;
	return desiredFrameNumber;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int64_t clMediaFile::ConvertSecondsToAudioPts(double seekPosInSeconds) const
{
	int64_t tsms = (int64_t)(seekPosInSeconds * 1000.0f);
	return ConvertMSToAudioPts(tsms);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int64_t clMediaFile::ConvertMSToAudioPts(int64_t tsms) const
{
	int64_t desiredFrameNumber = av_rescale(tsms, m_pFormatContext->streams[m_nAudioStreamIdx]->time_base.den, m_pFormatContext->streams[m_nAudioStreamIdx]->time_base.num);
	desiredFrameNumber /= 1000;
	return desiredFrameNumber;
}

//----------------------------------------------------------------------------------------------------------------------
//  Returns time of keyframe that we seeked to in seconds, returns time of new frame in seconds
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::SeekToNextKeyFrame(const clTimeStamp& seekPos)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekToNextKeyFrame(%d)\n", seekPos.GetSec());

	// Check that we have index entries
	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
	if (pStream->nb_index_entries <= 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "Media does not have key frames.\n");
		return clTimeStamp::s_undefined;
	}

	// Check that there is a keyframe after desired seek position
	int64_t desiredFramePts = ConvertSecondsToVideoPts(seekPos.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;

	int keyIndex = av_index_search_timestamp(pStream, desiredFrameDts, 0);
	if (keyIndex < 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "no keyframe after %i (time %f) found in index.\n", desiredFramePts, seekPos.GetSec());
		// Seek backwards instead, if we don't return something time bender decoding can 
		// stop due to EOF or seek forward frame-by-frame
		keyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
		if (keyIndex < 0)
		{
			FFMPEGUtils::Log(AV_LOG_WARNING, "No keyframe found either forwards or backwards!\n");
			return clTimeStamp::s_undefined;
		}
	}

	// Perform the seek
	int ret = _SeekFrame(m_nVideoStreamIdx, pStream->index_entries[keyIndex].timestamp, 0);
	if (ret < 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "failed to seek to keyframe at time: %i.\n", pStream->index_entries[keyIndex].timestamp);
		return clTimeStamp::s_undefined;
	}

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	return clTimeStamp(ConvertVideoPtsToMS(pStream->index_entries[keyIndex].timestamp - pStream->first_dts));
}

//----------------------------------------------------------------------------------------------------------------------
//  Returns time of keyframe that we seeked to in seconds
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::SeekToPrevKeyFrame(const clTimeStamp& seekPos)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekToPrevKeyFrame(%d)\n", seekPos.GetSec());

	// Check that we have index entries
	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
	if (pStream->nb_index_entries <= 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "Media does not have key frames.\n");
		return clTimeStamp::s_undefined;
	}

	// Check that there is a keyframe before desired seek position
	int64_t desiredFramePts = ConvertSecondsToVideoPts(seekPos.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;

	int keyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
	if (keyIndex < 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "no keyframe before %i (time %f) found in index.\n", desiredFramePts, seekPos.GetSec());
		return clTimeStamp::s_undefined;
	}

	// Perform the seek
	int ret = _SeekFrame(m_nVideoStreamIdx, pStream->index_entries[keyIndex].timestamp, 0);
	if (ret < 0)
	{
		FFMPEGUtils::Log(AV_LOG_WARNING, "failed to seek to keyframe at time: %i.\n", pStream->index_entries[keyIndex].timestamp);
		return clTimeStamp::s_undefined;
	}

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	return clTimeStamp(ConvertVideoPtsToMS(pStream->index_entries[keyIndex].timestamp - pStream->first_dts));
}

//----------------------------------------------------------------------------------------------------------------------
//  Get the time stamp from the prior keyframe
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetPriorVideoKeyTimeStamp(const clTimeStamp& referenceTime) const
{
	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
	int64_t desiredFramePts = ConvertSecondsToVideoPts(referenceTime.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;
	int prevKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
	if (prevKeyIndex >= 0)
	{
		return clTimeStamp(ConvertVideoPtsToMS(pStream->index_entries[prevKeyIndex].timestamp - pStream->first_dts));
	}
	return clTimeStamp::s_zero;
}

//----------------------------------------------------------------------------------------------------------------------
//  Get the time stamp from the next keyframe
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetNextVideoKeyTimeStamp(const clTimeStamp& referenceTime) const
{
	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
	int64_t desiredFramePts = ConvertSecondsToVideoPts(referenceTime.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;
	int nextKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, 0);
	if (nextKeyIndex >= 0)
	{
		return clTimeStamp(ConvertVideoPtsToMS(pStream->index_entries[nextKeyIndex].timestamp - pStream->first_dts));
	}
	return referenceTime;
}

//----------------------------------------------------------------------------------------------------------------------
//  Get the keyframe time stamp closest to given time
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetClosestVideoKeyTimeStamp(const clTimeStamp& referenceTime) const
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetClosestVideoKeyTimeStamp(%d)\n", referenceTime.GetSec());

	//	NM_ASSERT(seekPosInSeconds <= GetDurationInSeconds());  This assertion triggers too often for me 
	NM_ASSERT(m_nVideoStreamIdx >= 0);

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	int64_t desiredFramePts = ConvertSecondsToVideoPts(referenceTime.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;

	// Choose closest keyframe
	int64_t deltaPrev = 10000000;
	int64_t deltaNext = 10000000;
	bool foundIndex = false;

	int prevKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
	if (prevKeyIndex >= 0)
	{
		foundIndex = true;
		deltaPrev = abs(pStream->index_entries[prevKeyIndex].timestamp - desiredFrameDts);
	}

	int nextKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, 0);
	if (nextKeyIndex >= 0)
	{
		foundIndex = true;
		deltaNext = abs(pStream->index_entries[nextKeyIndex].timestamp - desiredFrameDts);
	}

	if (!foundIndex)
	{
		return clTimeStamp::s_zero;
	}
	else
	{
		int index = deltaPrev < deltaNext ? prevKeyIndex : nextKeyIndex;
		return clTimeStamp(ConvertVideoPtsToMS(pStream->index_entries[index].timestamp - pStream->first_dts));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  Get the time stamp from the prior keyframe
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetPriorAudioKeyTimeStamp(const clTimeStamp& referenceTime) const
{
	int64_t desiredFramePts = ConvertSecondsToAudioPts(referenceTime.GetSec());
	AVStream *st = m_pFormatContext->streams[m_nAudioStreamIdx];
	int prevKeyIndex = av_index_search_timestamp(st, desiredFramePts, AVSEEK_FLAG_BACKWARD);
	if (prevKeyIndex >= 0)
	{
		return clTimeStamp(ConvertAudioPtsToMS(st->index_entries[prevKeyIndex].timestamp));
	}
	return clTimeStamp::s_zero;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::SeekToClosestVideoKeyFrame(std::shared_ptr<clMediaFile::clVideoFrame> frame)
{
	SeekToClosestVideoKeyFrame(frame, clTimeStamp(ConvertVideoPtsToMS(m_nCurrentPts)));
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::SeekToClosestVideoKeyFrame(std::shared_ptr<clMediaFile::clVideoFrame> frame, const clTimeStamp& seekPos)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekToClosestVideoKeyFrame(%d)\n", seekPos.GetSec());

//	NM_ASSERT(seekPosInSeconds <= GetDurationInSeconds());  This assertion triggers too often for me 
	NM_ASSERT(m_nVideoStreamIdx >= 0);

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	int64_t desiredFramePts = ConvertSecondsToVideoPts(seekPos.GetSec());// -m_nFirstPts; now done to frames after decode 
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	// Choose closest keyframe
	int64_t deltaPrev = 10000000;
	int64_t deltaNext = 10000000;

	int prevKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
	if (prevKeyIndex >= 0)
	{
		deltaPrev = abs(pStream->index_entries[prevKeyIndex].timestamp - desiredFrameDts);
	}

	int nextKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, 0);
	if (nextKeyIndex >= 0)
	{
		deltaNext = abs(pStream->index_entries[nextKeyIndex].timestamp - desiredFrameDts);
	}
	int index = deltaPrev < deltaNext ? prevKeyIndex : nextKeyIndex;

	// Seek stream to this spot
	int ret = _SeekFrame(m_nVideoStreamIdx, pStream->index_entries[index].timestamp, 0);
	if (ret < 0)
	{
		// Something went wrong, try normal seek
		char errorString[1024];
		av_make_error_string(errorString, 1024, ret);
		SeekVideo(frame, seekPos);
	}
	else
	{
		// Next frame is a keyframe
		GetNextVideoFrame(frame);
	}
}

//----------------------------------------------------------------------------------------------------------------------
// Seek to an exact spot in the media file. This is potentially slow because you can't seek directly
// to the desired frame.  Decoder must start at a keyframe and then decode until desired frame is found.
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::SeekVideo(std::shared_ptr<clMediaFile::clVideoFrame> frame, const clTimeStamp& seekPos)
{
	//NM_TRACESIMPLE("Seek video %f\n", seekPos.GetSec());
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekVideo(%d)\n", seekPos.GetSec());

	NM_ASSERT(m_nVideoStreamIdx >= 0);

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	int64_t desiredFramePts = ConvertSecondsToVideoPts(seekPos.GetSec()); // -m_nFirstPts; now done to frames after decode
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;
	//FFMPEGUtils::Log(AV_LOG_INFO, "Seek video to %i %i\n", desiredFramePts, ConvertVideoPtsToMS(desiredFramePts));

	// TODO:  Is this next check correct or necessary?  That's a function pointer...
	if (m_pFormatContext->iformat->read_seek)
	{
		// Find the keyframe prior to our desired frame
		int prevKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
		if (prevKeyIndex >= 0)
		{
			_SeekFrame(m_nVideoStreamIdx, pStream->index_entries[prevKeyIndex].timestamp, AVSEEK_FLAG_BACKWARD);
			// Walk forward to desired frame
			_SeekVideoFrameByFrame(frame, seekPos);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
// Wrapper for av_seek_frame so that they all go through the same spot.  This simplifies debugging and
// flagging the video decoder to reset.
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::_SeekFrame(int stream_index, int64_t timestamp, int flags)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_SeekFrame(%d, %d, %d)\n", stream_index, timestamp, flags);
	//NM_TRACESIMPLE("Clearing packets and Performing seek to %f %p\n", 0.001*ConvertVideoPtsToMS(timestamp), this);

	_ClearPacketQueues();
	
	// Since we're doing a seek here, we won't need to do one when re-opening the video context
	m_reOpenSeekTime.SetMs(-1.0);
	int rVal = av_seek_frame(m_pFormatContext, stream_index, timestamp, flags);
	if (rVal < 0)
	{
		PrintAVError(rVal, "_SeekFrame");
	}
	else
	{
		_Flush();
	}

	return rVal;
}

//----------------------------------------------------------------------------------------------------------------------
// Seek to an exact spot in the media file. 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::SeekVideo(const clTimeStamp& seekPos)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekVideo(%d)\n", seekPos.GetSec());

	NM_ASSERT(m_nVideoStreamIdx >= 0);

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	int64_t desiredFramePts = ConvertSecondsToVideoPts(seekPos.GetSec()); // -m_nFirstPts; now done to frames after decode
	int64_t desiredFrameDts = desiredFramePts + pStream->first_dts;
	//FFMPEGUtils::Log(AV_LOG_INFO, "Seek video to %i %i\n", desiredFramePts, ConvertVideoPtsToMS(desiredFramePts));

	// TODO:  Is this next check correct or necessary?  That's a function pointer... 
	if (m_pFormatContext->iformat->read_seek)
	{
		// Find the keyframe prior to our desired frame
		int prevKeyIndex = av_index_search_timestamp(pStream, desiredFrameDts, AVSEEK_FLAG_BACKWARD);
		if (prevKeyIndex >= 0)
		{
			_SeekFrame(m_nVideoStreamIdx, pStream->index_entries[prevKeyIndex].timestamp, AVSEEK_FLAG_BACKWARD);
		}
		else
		{
			NM_TRACESIMPLE("Warning: Did not seek prevkeyIndex = %i\n", prevKeyIndex);
		}
	}
	else
	{
		NM_TRACESIMPLE("Warning, did not seek, read_seek null\n");
	}
}


//----------------------------------------------------------------------------------------------------------------------
// Seek to a spot at or before the given time in the audio stream.  
// Decoding will proceed from there.  Exact timing should be restored when
// grabbing the data out of the audio ring buffer.
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::SeekToAudioFrame(const clTimeStamp& seekPos)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::SeekToAudioFrame(%d)\n", seekPos.GetSec());

	if (m_nAudioStreamIdx < 0)
	{
		return;
	}

	m_bEOF = false;
	m_bFinishedVideo = false;
	m_bFinishedAudio = false;
	m_bEndVideo = false;
	m_bEndAudio = false;

	int64_t desiredFramePts = ConvertSecondsToAudioPts(seekPos.GetSec()) + m_nFirstPts;  // Add first pts, outside program thinks everyhing starts at 0

	AVStream* pStream = m_pFormatContext->streams[m_nAudioStreamIdx];

	// Seek to a point at or before the requested time
	// Note: Using this direct seek can sometimes cause the audio to be off a bit compared to the video
	// when played back from different points in the time line, so seek to prior keyframe below
	// ret = _SeekFrame(m_audioStreamIdx, desiredFramePts, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD);

	int prevKeyIndex = av_index_search_timestamp(pStream, desiredFramePts, AVSEEK_FLAG_BACKWARD);
	int ret = 0;
	// The delta makes sure that the keyframe is not forward (because we want a previous keyframe)
	// or too far back (because decode won't catch up) for a keyframe.
	double delta = ConvertAudioPtsToMS(desiredFramePts - pStream->index_entries[prevKeyIndex].timestamp);
	int64_t actualSeekPts;
	if (prevKeyIndex >= 0 && (delta > 0.0 && delta < 10.0 * nmAudio::clAudioManager::GrainSizeToGrainTimeMs(m_audioParams.nGrainSize, m_audioParams.nSampleRate)))
	{
		actualSeekPts = pStream->index_entries[prevKeyIndex].timestamp;
		ret = _SeekFrame(m_nAudioStreamIdx, actualSeekPts, AVSEEK_FLAG_BACKWARD);
	}
	else
	{
		actualSeekPts = desiredFramePts;
		ret = _SeekFrame(m_nAudioStreamIdx, actualSeekPts, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD);
	}

	if (ret < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "%s: error while seeking audio in %s\n", av_err2str(ret), m_strFilename.c_str());
	}
	else
	{
		// Flush the stream to clear any current data
		avcodec_flush_buffers(m_pAudioCodecContext);
		if (prevKeyIndex >= 0)
		{
			m_nCurrentAudioPts = actualSeekPts;
		}
		else
		{
			m_nCurrentAudioPts = actualSeekPts;
		}

		// FFMPEGUtils::Log(AV_LOG_INFO, "Seek to audio pts: %i ms: %f\n", resultPts, ConvertAudioPtsToMS(resultPts));
	}
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetVideoDuration()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetVideoDuration()\n");

	if (m_nVideoStreamIdx < 0)
	{
		return clTimeStamp::s_zero;
	}

	// This is the method recommended by stack overflow which is never wrong.  It's simpler than the 
	// ones below and I am not so confident about the time index method so give it a try for a bit 
	clTimeStamp duration(m_pFormatContext->duration * (1000.0 / AV_TIME_BASE));
	return duration;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetAudioDuration()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetAudioDuration()\n");

	if (m_nAudioStreamIdx < 0)
	{
		return clTimeStamp::s_zero;
	}

	double rational = ((double)(m_pFormatContext->streams[m_nAudioStreamIdx]->time_base.num)) / ((double)(m_pFormatContext->streams[m_nAudioStreamIdx]->time_base.den));
	return clTimeStamp(1000.0*((double)m_pFormatContext->streams[m_nAudioStreamIdx]->duration)*rational);
}

//----------------------------------------------------------------------------------------------------------------------
//  Duration of a single frame
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetVideoFrameDuration() const
{
	if (m_nVideoStreamIdx < 0)
	{
		return clTimeStamp::s_zero;
	}

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];
	return clTimeStamp(1000.0 * double(pStream->avg_frame_rate.den) / double(pStream->avg_frame_rate.num));
}

//----------------------------------------------------------------------------------------------------------------------
//  Duration of a single frame
//----------------------------------------------------------------------------------------------------------------------
clTimeStamp clMediaFile::GetAudioFrameDuration() const
{
	// The only way to get this is to decode some frames, but all music I tried is 24 ms (9216 bytes) per frame.
	// I believe this is only used for music trim screen anyway 
	return clTimeStamp(24.0);
}

//----------------------------------------------------------------------------------------------------------------------
//  Get interval between keyframes.  Not always constant, thus min and max.
//  I have tried several different methods for this in the past but nothing other than inspecting 
//  the packets is reliable.  Return value is false on failure
//----------------------------------------------------------------------------------------------------------------------
bool clMediaFile::ScanKeyframeIntervalRange( int& minKeyInt, int& maxKeyInt )
{
	minKeyInt = 10000;
	maxKeyInt = 0;

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	// Method 1:  Query gop_size (apparently not the same as keyframe interval)
	/*
		AVCodecContext* pVideoCodecContext = GetVideoContext(false);
		minKeyInt = maxKeyInt = pStream->gop_size;
		return true;
	*/

	// Method 2: Look for keyframes in index (not reliable, index can be incomplete or missing, which is also an issue for us)
	/*
	for (int i = 1, keyInt = 0; i < pStream->nb_index_entries; ++i)
	{
		++keyInt;
		if (pStream->index_entries[i].flags & AVINDEX_KEYFRAME)
		{
			minKeyInt = std::min(keyInt, minKeyInt);
			maxKeyInt = std::max(keyInt, maxKeyInt);
			keyInt = 0;
		}
	}
	return true;
	*/

	// Method 3: Seek to start then read video packets until we've hit 10 keyframes 
	int nRet = 0;
	int keysHit = 0;
	if (_SeekFrame(m_nVideoStreamIdx, pStream->index_entries[0].timestamp, AVSEEK_FLAG_BACKWARD) >= 0)
	{
		//double startTime = nmSystem::GetCoreTimeMS();
		AVPacket* pPacket = av_packet_alloc();
		NM_ASSERTPTR(pPacket);
		av_init_packet(pPacket);

		int keyIntCounter = 0;
		int packetsRead = 0;
		do
		{
			nRet = av_read_frame(m_pFormatContext, pPacket);
			++packetsRead;
			if (nRet >= 0)
			{
				if (pPacket->stream_index == m_nVideoStreamIdx)
				{
					if (pPacket->flags & AV_PKT_FLAG_KEY)
					{
						++keysHit;
						if (keysHit > 1)  // ignore first keyframe
						{
							minKeyInt = std::min(keyIntCounter, minKeyInt);
							maxKeyInt = std::max(keyIntCounter, maxKeyInt);
						}
						keyIntCounter = 0;
					}
					++keyIntCounter;
				}
			}
			av_packet_unref(pPacket);
		} while (keysHit < 20 && nRet >= 0 && packetsRead < 500);
		
		//av_packet_unref(pPacket); // av_packet_free calls av_packet_unref
		av_packet_free(&pPacket);
		//double endTime = nmSystem::GetCoreTimeMS();

		//NM_TRACESIMPLE("Found keyframe interval range %i to %i after %i intervals in %f ms\n", minKeyInt, maxKeyInt, keysHit, endTime-startTime);
		_SeekFrame(m_nVideoStreamIdx, pStream->index_entries[0].timestamp, AVSEEK_FLAG_BACKWARD);
	}
	return keysHit > 1;
}

//----------------------------------------------------------------------------------------------------------------------
//  Audio codec used for this media
//----------------------------------------------------------------------------------------------------------------------
AVCodecID clMediaFile::GetAudioCodecId() const
{
	AVCodecID nCodecId = AV_CODEC_ID_NONE;
	if (m_pAudioCodecContext != nullptr)
	{
		nCodecId = m_pAudioCodecContext->codec_id;
	}
	return nCodecId;
}

//----------------------------------------------------------------------------------------------------------------------
//  Video codec used for this media
//----------------------------------------------------------------------------------------------------------------------
AVCodecID clMediaFile::GetVideoCodecId() const
{
	return m_nVideoCodecId;
}

//----------------------------------------------------------------------------------------------------------------------
// Open a media file and generate a thumbnail
//----------------------------------------------------------------------------------------------------------------------
bool clMediaFile::GenerateThumbnailFrame(const char* filename, const clTimeStamp& startPos, std::shared_ptr<clVideoFrame> thumb)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GenerateThumbnailFrame()\n");

	clMediaFile mediaFile(false);
	bool bRet = false;

	if( mediaFile.OpenFile(filename,kVideoStream) < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Thumbnails not generated could not open file.\n");
		return bRet;
	}
	//NM_TRACESIMPLE("Generating thumb frame for %f\n", startPos.GetSec());
	mediaFile.SeekToClosestVideoKeyFrame(thumb, startPos);

	if (thumb->IsValid())
	{
		bRet = true;
	}
	else
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "generating thumb for %s\n", filename);
	}

	mediaFile.Close();

	return bRet;
}

//----------------------------------------------------------------------------------------------------------------------
// Open a media file and generate an array of thumbnails
//----------------------------------------------------------------------------------------------------------------------
bool clMediaFile::GenerateThumbnailFrames(const char* filename, const clTimeStamp& startPos,
	const clTimeStamp& endPos, std::vector<std::shared_ptr<clVideoFrame>>& vecFrames)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GenerateThumbnailFrames()\n");

	clMediaFile mediaFile(false);
	bool bRet = false;
	int nFrames = int(vecFrames.size());

	if (nFrames <= 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Thumbnails not generated because output array has size 0.\n");
		return bRet;
	}

	if (mediaFile.OpenFile(filename, kVideoStream) < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Thumbnails not generated could not open file.\n");
		return bRet;
	}

	clTimeStamp step((endPos - startPos).GetMs() / double(nFrames));
	// Take actual frame samples at the center of each subsection (add half a step)
	clTimeStamp currentPos = startPos + step * 0.5;

	bRet = true;
	for (int i = 0; i < nFrames; ++i, currentPos += step)
	{
		mediaFile.SeekToClosestVideoKeyFrame(vecFrames[i], currentPos);
		if (!vecFrames[i]->IsValid())
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "generating thumb %i for %s\n", i, filename);
			bRet = false;
			break;
		}
	}

	mediaFile.Close();

	return bRet;
}

//----------------------------------------------------------------------------------------------------------------------
// Open a media file and generate a thumbnail, attempting to find a "good" one.  
// This is more expensive than generating one for a specific time, obviously ;)
//----------------------------------------------------------------------------------------------------------------------
bool clMediaFile::GenerateGoodThumbnailFrame(const char* filename, const clTimeStamp& startPos, const clTimeStamp& endPos, std::shared_ptr<clVideoFrame> thumb)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GenerateThumbnailFrame()\n");

	clMediaFile mediaFile;
	bool bRet = false;

	double startTimer = nmSystem::GetCoreTimeMS();

	if (mediaFile.OpenFile(filename, kVideoStream) < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Thumbnails not generated could not open file.\n");
		return bRet;
	}

	const int sizeThreshold = 100000;
	std::shared_ptr<clVideoFrame> maxThumb;

	clTimeStamp startTime = std::max(startPos, clTimeStamp::s_zero);
	clTimeStamp endTime = std::min(endPos, mediaFile.GetVideoDuration());
	clTimeStamp step = std::max(clTimeStamp(1000.0), (endTime - startTime) * 0.1);
	int count = 0;
	for (clTimeStamp time = startTime; time < endTime; time += step, ++count )
	{
		mediaFile.SeekToClosestVideoKeyFrame(thumb, time);
		int size = thumb->GetPacketSize();
		if (size > sizeThreshold)
		{
			NM_TRACESIMPLE("Thumbnail found via threshold %i\n", size);
			maxThumb = thumb;
			break;
		}
		else if (maxThumb == nullptr || size > maxThumb->GetPacketSize())
		{
			NM_TRACESIMPLE("Thumbnail set to %i\n", size);
			maxThumb = thumb;
		}
	}
	double endTimer = nmSystem::GetCoreTimeMS();
	NM_TRACESIMPLE("Good thumb found in %f ms after %i tries\n", endTimer - startTimer, count);


	if (maxThumb != nullptr)
	{
		thumb = maxThumb;
	}

	if (thumb->IsValid())
	{
		bRet = true;
	}
	else
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "generating thumb for %s\n", filename);
	}

	mediaFile.Close();

	return bRet;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetAudioSampleRate()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetAudioSampleRate()\n");

	if(m_pAudioCodecContext != nullptr)
		return m_pAudioCodecContext->sample_rate;
	else
		return -1;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetAudioDecodeFrameSize()
{
	//FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::GetAudioDecodeFrameSize()\n");

	if (m_pAudioCodecContext != nullptr)
	{
		float upscaleRatio = float(m_audioParams.nSampleRate) / (float)m_pAudioCodecContext->sample_rate;
		
		// Some audio files (namely Ogg), don't set the frame_size member.  
		// The 2048 was chosen to be larger than any that I've seen so that the 
		// ring buffer has enough space when the audio comes out.  If I'm wrong and
		// larger packets come in, they will be dropped. 
		float frameSize = m_pAudioCodecContext->frame_size > 0 ? m_pAudioCodecContext->frame_size : 2048.0f;
		
		// The 4.0 below is for float, the 2.0 is for 2 channels, we always convert output to stereo
		float size = frameSize * upscaleRatio * 4.0f * 2.0f; //  (float)m_pAudioCodecContext->channels;
		return (int)size;
	}
	else
		return -1;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetAudioNumChannels()
{
	if(m_pAudioCodecContext != nullptr)
		return m_pAudioCodecContext->channels;
	else
		return -1;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::GetAudioSampleCount()
{
	if(m_pAudioCodecContext != nullptr)
		return m_pAudioCodecContext->frame_size;
	else
		return -1;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::ClearYuv2(uint8_t* pOut, int nWidth, int nHeight)
{
	memset(pOut, 0, nWidth*nHeight*2);
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::DecodeFrame(AVCodecContext * pCodecContext, AVFrame * pFrame, int * pGotFrame, const AVPacket * pPkt)
{
	int ret;

	*pGotFrame = 0;

	if (pPkt)
	{
		do
		{
			if (pPkt->size == 0)
			{
				ret = avcodec_send_packet(pCodecContext, nullptr);
				FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_send_packet() -> Flush Returned: 0x%x\n", ret);
			}
			else
			{
				ret = avcodec_send_packet(pCodecContext, pPkt);
			}

			// input is not accepted in the current state - user must read output with avcodec_receive_frame()
			// (once all output is read, the packet should be resent, and the call will not fail with EAGAIN)
			if (ret == AVERROR(EAGAIN))
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_send_packet() Returned: AVERROR(EAGAIN)\n");
				return ret;
			}
			// the decoder has been flushed, and no new packets can be sent to it (also returned if more than 1 flush packet is sent)
			else if (ret == AVERROR_EOF)
			{
				avcodec_flush_buffers(pCodecContext);
				FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_send_packet() Returned: AVERROR_EOF\n");
				break;
				//return ret;
			}
			else if (ret < 0)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_send_packet() Returned: 0x%x\n", ret);
				return ret;
			}
		} while (ret == AVERROR(EAGAIN));
	}

	do
	{
		ret = avcodec_receive_frame(pCodecContext, pFrame);
		if (ret >= 0)
		{
			*pGotFrame = 1;
			return 0;
		}
		// output is not available in this state - user must try to send new input
		else if (ret == AVERROR(EAGAIN))
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_receive_frame() Returned: AVERROR(EAGAIN)\n");
			return ret;
		}
		// the decoder has been fully flushed, and there will be no more output frames
		else if (ret == AVERROR_EOF)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_receive_frame() Returned: AVERROR_EOF\n");
			return ret;
		}
		else
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "clMediaFile::DecodeFrame() -> avcodec_receive_frame() Returned: 0x%x\n", ret);
			return ret;
		}
	} while (ret == AVERROR(EAGAIN));
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::EncodePacket(AVCodecContext * pCodecContext, AVPacket * pPkt, const AVFrame * pFrame, int * pGotFrame)
{
	int rVal = avcodec_send_frame(pCodecContext, pFrame);
	if (rVal < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "EncodePacket() -> avcodec_send_frame() Returned: 0x%x\n", rVal);
	}
	else
	{
		rVal = avcodec_receive_packet(pCodecContext, pPkt);
		if (rVal == AVERROR(EAGAIN) || rVal == AVERROR_EOF)
		{
			rVal = 0;
			*pGotFrame = false;
		}
		else if (rVal < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "EncodePacket() -> avcodec_receive_packet() Returned: 0x%x\n", rVal);
			*pGotFrame = false;
		}
		else
		{
			*pGotFrame = true;
		}
	}

	return rVal;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
int clMediaFile::CodecToThreadCount(AVCodec * const codec)
{
	int threadCount = 1;
	if (m_mapCodecToThreadCount.find(codec->name) != m_mapCodecToThreadCount.end())
	{
		threadCount = m_mapCodecToThreadCount[codec->name];
	}
	return threadCount;
}

//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eOpenFileStatus clMediaFile::_OpenCodecContext(enum AVMediaType type, int & stream_idx, AVCodecContext *& pCodecContext)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_OpenCodecContext(%s)\n", FFMPEGUtils::AVMediaTypeToString(type).c_str());

	if (type != AVMEDIA_TYPE_VIDEO && type != AVMEDIA_TYPE_AUDIO)
	{
		return kOpenFileStatus_UnsupportedMediaType;
	}

	int ret = av_find_best_stream(m_pFormatContext, type, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Could not find %s stream in input file '%s'\n", av_get_media_type_string(type), m_strFilename.c_str());
		return (AVMEDIA_TYPE_VIDEO == type) ? kOpenFileStatus_NoVideoStream : kOpenFileStatus_NoAudioStream;
	}
	else
	{
		stream_idx = ret;
		AVStream* pStream = m_pFormatContext->streams[stream_idx];

		AVCodec* pDecoder = avcodec_find_decoder(pStream->codecpar->codec_id);
		if (!pDecoder)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to find %s codec\n", av_get_media_type_string(type));
			return (AVMEDIA_TYPE_VIDEO == type) ? kOpenFileStatus_UnknownVideoCodec : kOpenFileStatus_UnknownAudioCodec;
		}

		FFMPEGUtils::Log(AV_LOG_INFO, "Stream[%i] Decoder: %s\n", stream_idx, pDecoder->name);

		// allocate a codec context for the decoder
		pCodecContext = avcodec_alloc_context3(pDecoder);
		if (!pCodecContext)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to allocate the codec context for stream #%u\n", stream_idx);
			return (AVMEDIA_TYPE_VIDEO == type) ? kOpenFileStatus_CouldNotOpenVideoCodec : kOpenFileStatus_CouldNotOpenAudioCodec;
		}

		// copy the codec parameters to the codec context
		int ret = avcodec_parameters_to_context(pCodecContext, pStream->codecpar);
		if (ret < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to copy codec parameters to input codec context for stream #%u\n", stream_idx);
			return (AVMEDIA_TYPE_VIDEO == type) ? kOpenFileStatus_CouldNotOpenVideoCodec : kOpenFileStatus_CouldNotOpenAudioCodec;
		}

		if (pCodecContext->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			pCodecContext->framerate = av_guess_frame_rate(m_pFormatContext, pStream, nullptr);
		}

		pCodecContext->thread_count = CodecToThreadCount(pDecoder);

		ret = avcodec_open2(pCodecContext, pDecoder, nullptr);
		if (ret < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to open %s codec\n", av_get_media_type_string(type));
			return (AVMEDIA_TYPE_VIDEO == type) ? kOpenFileStatus_CouldNotOpenVideoCodec : kOpenFileStatus_CouldNotOpenAudioCodec;
		}
	}

	return kOpenFileStatus_Success;
}

//----------------------------------------------------------------------------------------------------------------------
// Resets the codec so it can be used again.  Call after a seek or after last frame has been taken out of the file.
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::_Flush()
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_Flush()\n");

	if (m_bHasVideo)
	{
		// This used to call GetVideoContext(), but now m_pVideoCodecContext is updated in LoadFramePacket 
		// to make thread safety easier.
		AVCodecContext* pVideoCodecContext = m_pVideoCodecContext;

		if (VALID_PTR(pVideoCodecContext))
		{
			avcodec_flush_buffers(pVideoCodecContext);
			if (pVideoCodecContext->codec_id == AV_CODEC_ID_H264)
			{
				H264SCEAContext* sceaCtx = (H264SCEAContext*)pVideoCodecContext->priv_data;
				sceaCtx->resetStream = true;
			}
			else if (pVideoCodecContext->codec_id == AV_CODEC_ID_VP9)
			{
				//VP9SCEAContext* sceaCtx = (VP9SCEAContext*)m_pVideoCodecContext->priv_data;
				//sceaCtx->resetStream = true;
			}
			else
			{
				NM_ASSERTMSG(false, "Unhandled codec id %i in clMediaFile::_Flush()\n", pVideoCodecContext->codec_id);
			}
		}
	}

	if (m_bHasAudio && VALID_PTR(m_pAudioCodecContext))
	{
		avcodec_flush_buffers(m_pAudioCodecContext);
	}

	// TODO: When is avformat_flush needed?
	//avformat_flush(m_pFormatContext);
}

//----------------------------------------------------------------------------------------------------------------------
// Resets the codec so it can be used again.  Call after a seek or after last frame has been taken out of the file.
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::_Flush(AVCodecContext* pContext)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_Flush(pContext)\n");
	avcodec_flush_buffers(pContext);
}

//----------------------------------------------------------------------------------------------------------------------
// Step frame by frame until the desired frame is reached
//----------------------------------------------------------------------------------------------------------------------
void clMediaFile::_SeekVideoFrameByFrame(std::shared_ptr<clMediaFile::clVideoFrame> frame, const clTimeStamp& targetTime )
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_SeekVideoFrameByFrame(%f)\n", targetTime.GetMs());

	do
	{
		// Short circuit decode process
		if (m_bShortCircuitDecoding)
			break;

		//FFMPEGUtils::Log(AV_LOG_ERROR, "Decoding forward to desired position %d currently at %d\n",desiredFrameNumber, pts);
		GetNextVideoFrame(frame);
		//FFMPEGUtils::Log(AV_LOG_INFO, "Get next video frame moved us to %i, finished %i\n", frame->m_nFramePts, int(GetIsFinished()));
		//NM_TRACESIMPLE("frame time: %f desired: %f\n", frame->GetTimeStamp().GetSec(),  targetTime.GetSec());
	} while (frame->IsValid() && frame->GetTimeStamp() < targetTime && !GetIsVideoFinished());
}

//----------------------------------------------------------------------------------------------------------------------
// Get the video context for this file.  May come from the pool manaed by videoContextWrapper
//----------------------------------------------------------------------------------------------------------------------
AVCodecContext* clMediaFile::GetVideoContext(bool createIfNeeded)
{
	if (m_bUseContextWrapper)
	{
		return s_videoContextWrapper.GetVideoContext(this, createIfNeeded);
	}
	else
	{
		return m_pVideoCodecContext;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  We decided that we will no longer copy video files, we will always convert.  This allowed us to 
//  remove some tests that would otherwise be necessary for real-time playback
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eVideoStatus clMediaFile::_HasValidH264VideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_HasValidH264VideoAttributes(%d, %f, %f)\n", videoStatus, nMinDuration.GetSec(), nMaxDuration.GetSec());

	eVideoStatus nReturn = kVideoStatus_Success;

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	// This still needs to use GetVideoContext because it does not use threads to call LoadFramePacket
	AVCodecContext* pVideoCodecContext = GetVideoContext(true);

	// Check SHAREfactory constraints ------------------------
	{
		// Validate for minimum duration
		clTimeStamp nDuration = GetActualDuration();
		if (nDuration < nMinDuration || nDuration > nMaxDuration)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad Video duration: %i ms\n", nDuration.GetMsInt());
			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadDuration;

			videoStatus.SetFail(kVideoStatus_BadDuration);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadDuration);
		}

		// We expect that the video is YUV 4:2:0
		if (AV_PIX_FMT_NV12 != pVideoCodecContext->pix_fmt)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad chroma format: %i\n", pVideoCodecContext->pix_fmt);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadChromaFormat;

			videoStatus.SetFail(kVideoStatus_BadChromaFormat);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadChromaFormat);
		}

		// Make sure crop size is less than actual video size 
		if (pVideoCodecContext->coded_width < pVideoCodecContext->width
			|| pVideoCodecContext->coded_height < pVideoCodecContext->height)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad crop width/height: %i, %i\n", pVideoCodecContext->coded_width, pVideoCodecContext->coded_height);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadCrop;

			videoStatus.SetFail(kVideoStatus_BadCrop);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadCrop);
		}

		// Internally we are limited to resolutions of 1920x1080 or less
		if (pVideoCodecContext->width > 1920 || pVideoCodecContext->height > 1080)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad resolution, outside Sf range: %ix%i\n", pVideoCodecContext->width, pVideoCodecContext->height);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadResolution;

			videoStatus.SetFail(kVideoStatus_BadResolution);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadResolution);
		}

		// We only handle rotations in multiples of 90 degrees (give a little slop for floats)
		if (fabsf(fmodf(GetRotationAngleDegrees(), 90.0f)) > 0.0001f)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad rotation: %f\n", GetRotationAngleDegrees());

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadRotation;

			videoStatus.SetFail(kVideoStatus_BadRotation);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadRotation);
		}
	}

	// Check decoder constraints -------------------------------
	{
		// The file has to be a video
		if (AVMEDIA_TYPE_VIDEO != pVideoCodecContext->codec_type)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad codec type: %i\n", pVideoCodecContext->codec_type);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadType;

			videoStatus.SetFail(kVideoStatus_BadType);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadType);
		}

		// The file has to use h.264 codec
		if (AV_CODEC_ID_H264 != pVideoCodecContext->codec_id)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad codec id %i\n", pVideoCodecContext->codec_id);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadCodec;

			videoStatus.SetFail(kVideoStatus_BadCodec);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadCodec);
		}

		if (INVALID_PTR(pStream->codecpar))
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad pointer found in video data structures.\n");

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadPointer;

			videoStatus.SetFail(kVideoStatus_BadPointer);
			return nReturn;  // Unlike other errors we have to return right away if this one fails or we will crash
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadPointer);
		}		

		// I believe b-frames are placed in the decoded picture buffer
		// VideoDec2 supports a max of 4 decoded picture buffers
		if (4 < pVideoCodecContext->max_b_frames)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad b frames: %i\n", pVideoCodecContext->max_b_frames);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadBFrames;

			videoStatus.SetFail(kVideoStatus_BadBFrames);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadBFrames);
		}

		// Decoder is limited to resolutions in the range 64x64 to 1920x1088
		// NOTE: See second resolution check later on in the code, SHAREfactory constraints.
		if (pVideoCodecContext->width < 64 || pVideoCodecContext->width > 1920
		|| pVideoCodecContext->height < 64 || pVideoCodecContext->height > 1088)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad resolution, outside decoder range: %ix%i\n", pVideoCodecContext->width, pVideoCodecContext->height);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadResolution;

			videoStatus.SetFail(kVideoStatus_BadResolution);
		}
		else
		{
			// Make sure we didn't fail the previous resolution test
			videoStatus.SetPass(kVideoStatus_BadResolution);
		}

		// This is the maximum bit rate supported 
		if (pVideoCodecContext->bit_rate > 62500000)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad bit rate: %i\n", pVideoCodecContext->bit_rate);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadBitRate;

			videoStatus.SetFail(kVideoStatus_BadBitRate);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadBitRate);
		}

		// NOTE:  Always do this test last because it can be slow and if there is something unforseen it could
		//        crash the decoder.  If we can fail for any other reason we should.
		// Test to see if we can get a good frame back in a reasonable amount of time.
		// Some video streams are bad and we don't know until we try
		std::shared_ptr<clMediaFile::clVideoFrame> testFrame = std::make_shared<clMediaFile::clVideoFrameNoEncoding>();
		double decodeTime = getTimeMs();
		int result = GetNextVideoFrame(testFrame);
		decodeTime = getTimeMs() - decodeTime;

		// VideoDec2 docs say: "baseline/main/high profile up to level 5.1
		const int profile = pVideoCodecContext->profile;
		if (!(FF_PROFILE_H264_BASELINE == profile
			|| FF_PROFILE_H264_MAIN == profile
			|| FF_PROFILE_H264_HIGH == profile))
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad profile: %i\n", profile);
			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadProfile;

			videoStatus.SetFail(kVideoStatus_BadProfile);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadProfile);
		}

		// Check that no unimplemented code paths were taken in h264_scea.c
		H264SCEAContext* sceaCtx = (H264SCEAContext*)pVideoCodecContext->priv_data;
		if (sceaCtx->unsupportedParse)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Video stream parsing failed.\n");

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadParse;

			videoStatus.SetFail(kVideoStatus_BadParse);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadParse);
		}

		// Check for interlace failure first (it will also return result < 0 but we want a special message for it)
		if (!testFrame->GetProgressive())
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad video stream, interlaced instead of progressive scan\n");
			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadScanMode;

			videoStatus.SetFail(kVideoStatus_BadScanMode);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadScanMode);


			// This final test (of good frame back) has to be in the same condition as 
			// the progressive = true test or this message will override the bad video stream message
			if (result <= 0
				|| testFrame->GetWidth() == 0 // Can't check isValid because this noEncoding type doesn't store that info
				|| decodeTime > 1000.0)
			{
				FFMPEGUtils::Log(AV_LOG_ERROR, "Bad video stream, failed to decode\n");

				if (nReturn == kVideoStatus_Success)
					nReturn = kVideoStatus_BadStream;

				videoStatus.SetFail(kVideoStatus_BadStream);
			}
			else
			{
				videoStatus.SetPass(kVideoStatus_BadStream);
			}
		}
	}

	return nReturn;
}

//----------------------------------------------------------------------------------------------------------------------
//  We decided that we will no longer copy video files, we will always convert.  This allowed us to 
//  remove some tests that would otherwise be necessary for real-time playback
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eVideoStatus clMediaFile::_HasValidVP9VideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_HasValidVP9VideoAttributes(%d, %f, %f)\n", videoStatus, nMinDuration.GetSec(), nMaxDuration.GetSec());

	eVideoStatus nReturn = kVideoStatus_Success;

	AVStream* pStream = m_pFormatContext->streams[m_nVideoStreamIdx];

	// This still needs to use GetVideoContext because it does not use threads to call LoadFramePacket 
	AVCodecContext* pVideoCodecContext = GetVideoContext(true);

	// Check SHAREfactory constraints ------------------------
	{
		// Validate for duration
		clTimeStamp nDuration = GetActualDuration();
		if (nDuration < nMinDuration || nDuration > nMaxDuration)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad Video duration: %i ms\n", nDuration.GetMsInt());
			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadDuration;

			videoStatus.SetFail(kVideoStatus_BadDuration);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadDuration);
		}

		// We expect that the video is YUV 4:2:0, we don't look for other formats
		if ((AV_PIX_FMT_YUV420P != pVideoCodecContext->pix_fmt) && (AV_PIX_FMT_YUV420P10LE != pVideoCodecContext->pix_fmt))
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad chroma format: %i\n", pVideoCodecContext->pix_fmt);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadChromaFormat;

			videoStatus.SetFail(kVideoStatus_BadChromaFormat);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadChromaFormat);
		}

		// Make sure crop dimensions are less than actual video size 
		if (pVideoCodecContext->coded_width < pVideoCodecContext->width
			|| pVideoCodecContext->coded_height < pVideoCodecContext->height)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad crop width/height: %i, %i\n", pVideoCodecContext->coded_width, pVideoCodecContext->coded_height);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadCrop;

			videoStatus.SetFail(kVideoStatus_BadCrop);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadCrop);
		}

		// Make sure video doesn't exceed maximum resolutions.  These resolutions should correspond to frame texture pool dimensions initialized in AppFilmManager
		const int maxWidth = 3840;
		const int maxHeight = 2160;

		if (pVideoCodecContext->width > maxWidth || pVideoCodecContext->height > maxHeight)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad resolution, outside Sf range: %ix%i\n", pVideoCodecContext->width, pVideoCodecContext->height);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadResolution;

			videoStatus.SetFail(kVideoStatus_BadResolution);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadResolution);
		}

		// We only handle rotations in multiples of 90 degrees (give a little slop for floats)
		if (fabsf(fmodf(GetRotationAngleDegrees(), 90.0f)) > 0.0001f)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad rotation: %f\n", GetRotationAngleDegrees());

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadRotation;

			videoStatus.SetFail(kVideoStatus_BadRotation);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadRotation);
		}
	}

	// Check decoder constraints -------------------------------
	{
		// The file has to be a video
		if (AVMEDIA_TYPE_VIDEO != pVideoCodecContext->codec_type)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad codec type: %i\n", pVideoCodecContext->codec_type);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadType;

			videoStatus.SetFail(kVideoStatus_BadType);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadType);
		}

		// The file has to use vp9 codec
		if (AV_CODEC_ID_VP9 != pVideoCodecContext->codec_id)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad codec id %i\n", pVideoCodecContext->codec_id);

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadCodec;

			videoStatus.SetFail(kVideoStatus_BadCodec);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadCodec);
		}

		if (INVALID_PTR(pStream->codecpar))
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad pointer found in video data structures.\n");

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadPointer;

			videoStatus.SetFail(kVideoStatus_BadPointer);
			return nReturn;  // Unlike other errors we have to return right away if this one fails or we will crash
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadPointer);
		}

		// NOTE:  Always do this test last because it can be slow and if there is something unforseen it could
		//        crash the decoder.  If we can fail for any other reason we should.
		// Test to see if we can get a good frame back in a reasonable amount of time.
		// Some video streams are bad and we don't know until we try
		std::shared_ptr<clMediaFile::clVideoFrame> testFrame = std::make_shared<clMediaFile::clVideoFrameNoEncoding>();
		double decodeTime = getTimeMs();
		int result = GetNextVideoFrame(testFrame);
		decodeTime = getTimeMs() - decodeTime;

		if (result <= 0 || testFrame->GetWidth() == 0  || decodeTime > 1000.0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Bad video stream, failed to decode\n");

			if (nReturn == kVideoStatus_Success)
				nReturn = kVideoStatus_BadStream;

			videoStatus.SetFail(kVideoStatus_BadStream);
		}
		else
		{
			videoStatus.SetPass(kVideoStatus_BadStream);
		}
	}

	return nReturn;
}

//----------------------------------------------------------------------------------------------------------------------
// Constructor, initializes vp9 database
//----------------------------------------------------------------------------------------------------------------------
clVideoContextWrapper::clVideoContextWrapper()
{
	for (int i = 0; i < N_VIDEO_CONTEXTS; ++i)
	{
		m_aVp9Pool[i].m_pMediaFile = nullptr;
		m_aVp9Pool[i].m_pVideoCodecContext = nullptr;
		m_aVp9Pool[i].m_lastUseTime = clTimeStamp::s_zero;
	}
}

//----------------------------------------------------------------------------------------------------------------------
// Get video context for given media file.  Opens it if requested.
//----------------------------------------------------------------------------------------------------------------------
AVCodecContext* clVideoContextWrapper::GetVideoContext(clMediaFile* pMediaFile, bool createIfNeeded)
{
	if (!pMediaFile->GetHasVideo())
	{
		return nullptr;
	}

	for (int j = 0; j < 2; ++j)
	{
		// Is it H264
		auto iter = m_mapH264Contexts.find(pMediaFile);
		if (iter != m_mapH264Contexts.end())
		{
			return iter->second;
		}

		// Is it a VP9
		for (int i = 0; i < N_VIDEO_CONTEXTS; ++i)
		{
			if (m_aVp9Pool[i].m_pMediaFile == pMediaFile)
			{
				m_aVp9Pool[i].m_lastUseTime.SetMs(nmSystem::GetSystemTimeMS());
				return m_aVp9Pool[i].m_pVideoCodecContext;
			}
		}

		// Open and seek to correct spot
		if (createIfNeeded)
		{
			pMediaFile->LockStream();
			//NM_TRACESIMPLE("re-Opening video context to %f\n", pMediaFile->m_reOpenSeekTime.GetSec());
			Open(pMediaFile);
			createIfNeeded = false;

			// If there has been no seek since this file was closed, we need to do one here
			// To ensure that we're re-starting the video decoder on a keyframe
			if (pMediaFile->m_reOpenSeekTime >= clTimeStamp::s_zero )
			{
				pMediaFile->SeekVideo(pMediaFile->m_reOpenSeekTime);
			//	NM_TRACESIMPLE("Swapped video needed seek to %f\n", pMediaFile->m_reOpenSeekTime.GetSec());
			}
			else
			{
			//	NM_TRACESIMPLE("Swapped video did NOT need seek to %f\n", pMediaFile->m_reOpenSeekTime.GetSec());
			}
			pMediaFile->UnlockStream();
		}
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
// Same as below but gets context from media file
//----------------------------------------------------------------------------------------------------------------------
void clVideoContextWrapper::Close(clMediaFile* pMediaFile)
{
	Close(pMediaFile, GetVideoContext(pMediaFile, false));
}

//----------------------------------------------------------------------------------------------------------------------
//  Close video context, flushes it's used memory, and removes it from database
//----------------------------------------------------------------------------------------------------------------------
void clVideoContextWrapper::Close(clMediaFile* pMediaFile, AVCodecContext* pContext)
{
	if (!pMediaFile->GetHasVideo())
	{
		return;
	}

	pMediaFile->LockStream();

	auto iter = m_mapH264Contexts.find(pMediaFile);
	if (iter != m_mapH264Contexts.end())
	{
		// Remove from map
		m_mapH264Contexts.erase(pMediaFile);
	}
	else  // it's vp9
	{
		// Remove from list
		for (int i = 0; i < N_VIDEO_CONTEXTS; ++i)
		{
			if (m_aVp9Pool[i].m_pMediaFile == pMediaFile)
			{
				m_aVp9Pool[i].m_pMediaFile = nullptr;
				m_aVp9Pool[i].m_pVideoCodecContext = nullptr;
				m_aVp9Pool[i].m_lastUseTime = clTimeStamp::s_zero;
			}
		}

		// Clear memory ffmpeg is holding onto
		pMediaFile->_Flush();
		avformat_flush(pMediaFile->m_pFormatContext);
	}

	if (pContext != nullptr)
	{
		avcodec_free_context(&pContext);
		pMediaFile->m_pVideoCodecContext = nullptr;
	}

	av_packet_unref(&pMediaFile->m_pkt);
	av_frame_unref(pMediaFile->m_frame);
	av_frame_free(&pMediaFile->m_frame);

	// When we re-open this video context, we have to do a seek 
	pMediaFile->_ClearPacketQueues();

	pMediaFile->UnlockStream();
}

//----------------------------------------------------------------------------------------------------------------------
//  Open video context only.   Requires m_pFormatContext to be open and active.
//  Stores resulting context in database, closing oldest one if needed for space.
//----------------------------------------------------------------------------------------------------------------------
clMediaFile::eOpenFileStatus clVideoContextWrapper::Open(clMediaFile* pMediaFile)
{
	FFMPEGUtils::Log(AV_LOG_DEBUG, "clMediaFile::_OpenCodecContext(AVMEDIA_TYPE_VIDEO)\n");

	enum AVMediaType type = AVMEDIA_TYPE_VIDEO;
	AVCodecContext* pCodecContext = nullptr;

	int ret = av_find_best_stream(pMediaFile->m_pFormatContext, type, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Could not find %s stream in input file '%s'\n", av_get_media_type_string(type), pMediaFile->GetFilename().c_str());
		return clMediaFileBase::kOpenFileStatus_NoVideoStream;
	}
	else
	{
		pMediaFile->m_nVideoStreamIdx = ret;
		AVStream* pStream = pMediaFile->m_pFormatContext->streams[pMediaFile->m_nVideoStreamIdx];

		AVCodec* pDecoder = avcodec_find_decoder(pStream->codecpar->codec_id);
		if (!pDecoder)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to find %s codec\n", av_get_media_type_string(type));
			return clMediaFileBase::kOpenFileStatus_UnknownVideoCodec;
		}

		FFMPEGUtils::Log(AV_LOG_INFO, "Stream[%i] Decoder: %s\n", pMediaFile->m_nVideoStreamIdx, pDecoder->name);

		// allocate a codec context for the decoder
		pCodecContext = avcodec_alloc_context3(pDecoder);
		if (!pCodecContext)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to allocate the codec context for stream #%u\n", pMediaFile->m_nVideoStreamIdx);
			return clMediaFileBase::kOpenFileStatus_CouldNotOpenVideoCodec;
		}

		// copy the codec parameters to the codec context
		int ret = avcodec_parameters_to_context(pCodecContext, pStream->codecpar);
		if (ret < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to copy codec parameters to input codec context for stream #%u\n", pMediaFile->m_nVideoStreamIdx);
			return clMediaFileBase::kOpenFileStatus_CouldNotOpenVideoCodec;
		}

		if (pCodecContext->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			pCodecContext->framerate = av_guess_frame_rate(pMediaFile->m_pFormatContext, pStream, nullptr);
		}

		pCodecContext->thread_count = pMediaFile->CodecToThreadCount(pDecoder);

		ret = avcodec_open2(pCodecContext, pDecoder, nullptr);
		if (ret < 0)
		{
			FFMPEGUtils::Log(AV_LOG_ERROR, "Failed to open %s codec\n", av_get_media_type_string(type));
			return clMediaFileBase::kOpenFileStatus_CouldNotOpenVideoCodec;
		}
	}

	if (pCodecContext->codec_id == AV_CODEC_ID_VP9)
	{
		stVP9PoolItem* pLastItem = &m_aVp9Pool[0];
		for (int i = 1; i < N_VIDEO_CONTEXTS; ++i)
		{
			if (m_aVp9Pool[i].m_lastUseTime < pLastItem->m_lastUseTime)
			{
				pLastItem = &m_aVp9Pool[i];
			}
		}
		
		if  ( nullptr != pLastItem->m_pMediaFile
			&& nullptr != pLastItem->m_pVideoCodecContext 
			&& avcodec_is_open(pLastItem->m_pVideoCodecContext))
		{
			Close(pLastItem->m_pMediaFile, pLastItem->m_pVideoCodecContext);
		}
		pLastItem->m_pMediaFile = pMediaFile;
		pLastItem->m_pVideoCodecContext = pCodecContext;
		pLastItem->m_lastUseTime.SetMs(nmSystem::GetSystemTimeMS());
	}
	else
	{
		m_mapH264Contexts[pMediaFile] = pCodecContext;
	}

	pMediaFile->m_frame = av_frame_alloc();
	if (!pMediaFile->m_frame)
	{
		FFMPEGUtils::Log(AV_LOG_ERROR, "Could not allocate frame\n");
		return clMediaFileBase::kOpenFileStatus_MemoryAllocFailed;
	}

	// initialize packet, set data to nullptr, let the demuxer fill it 
	av_init_packet(&pMediaFile->m_pkt);
	pMediaFile->m_pkt.data = nullptr;
	pMediaFile->m_pkt.size = 0;

	// A Close() then Open() was leaving the EOF in a true state 
	pMediaFile->m_bEOF = false;
	pMediaFile->m_bFinishedVideo = false;
	pMediaFile->m_bFinishedAudio = false;
	pMediaFile->m_bEndVideo = false;
	pMediaFile->m_bEndAudio = false;

	return clMediaFileBase::kOpenFileStatus_Success;
}
