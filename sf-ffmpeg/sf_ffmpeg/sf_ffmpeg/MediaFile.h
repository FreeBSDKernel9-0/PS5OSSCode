//----------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------

#pragma once

#include <stdarg.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "FFMPEGUtils.h"

#include "Content/mecoMediaFile.h"

#include <Fifo.h>
#include <Common.h>		
#include <Thread.h>	

extern "C"
{
#include <libavformat/avformat.h>
#include <libavformat/file_scea.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/scePthread_frame.h>
#include <libavutil/imgutils.h>
}

class clMediaFile : public clMediaFileBase
{
	friend class clVideoContextWrapper;

public:

	struct stAudioParams
	{
		u32 nGrainSize;
		u32 nSampleRate;
	};

	struct stInitParams
	{
		FFMPEGUtils::TraceFP pTraceFP;
		FFMPEGUtils::ETraceLevel traceLevel;
		stAudioParams initAudioParams;
		MemSCEAInitParams initParamsMemSCEA;
		FileSCEAInitParams initParamsFileSCEA;
		SCEPThreadInitParams initParamsSCEPThread;
		H264SCEAInitParams initParamsH264SCEA;
		std::map<std::string, int> mapCodecToThreadCount;
	};

	PRX_INTERFACE static int Initialize(stInitParams& initData);

	PRX_INTERFACE clMediaFile(bool useContextWrapper = true);
	PRX_INTERFACE virtual ~clMediaFile();
	PRX_INTERFACE double ConvertVideoPtsToMS(int64_t pts) const;
	PRX_INTERFACE double ConvertAudioPtsToMS(int64_t pts) const;
	PRX_INTERFACE virtual double GetVideoPtsToMS() const;
	PRX_INTERFACE virtual double GetAudioPtsToMS() const;
	PRX_INTERFACE int64_t ConvertSecondsToVideoPts(double posInSeconds) const;
	PRX_INTERFACE int64_t ConvertSecondsToAudioPts(double posInSeconds) const;
	PRX_INTERFACE int64_t ConvertMSToVideoPts(int64_t ms) const;
	PRX_INTERFACE int64_t ConvertMSToAudioPts(int64_t ms) const;
	PRX_INTERFACE eOpenFileStatus OpenFile(const char* filename, int streamFlags);
	PRX_INTERFACE AVFrame* AllocateFrame() { return av_frame_alloc(); }
	PRX_INTERFACE virtual void FreeFrame(AVFrame** f) { av_frame_unref(*f); av_frame_free(f); }
	PRX_INTERFACE virtual void UnrefFrame(AVFrame* f) { av_frame_unref(f); }

	PRX_INTERFACE void Close();

	PRX_INTERFACE int LoadFramePacket(const clTimeStamp& clipEnd, bool bNeedsAudio);
	PRX_INTERFACE int DecodeVideoPacket(std::shared_ptr<clVideoFrame>& outFrame, stFramePacker& fp, const clTimeStamp& beginningCutoffTime, bool keyFrameOnly);
	PRX_INTERFACE int DecodeAudioPacket(clAudioFrame& outFrame, float timeScale, const clTimeStamp& clipStart);
	PRX_INTERFACE int GetCurrentBufferedVideoPacketCount() { return m_qVideoPacket.Size(); }
	PRX_INTERFACE int GetCurrentBufferedAudioPacketCount() { return m_qAudioPacket.Size(); }
	PRX_INTERFACE static int GetMaxVideoPacketsToBuffer() { return 60; }
	PRX_INTERFACE static int GetMaxAudioPacketsToBuffer() { return 120; }

	// Note: With new decode thread scheme, these functions that return a frame should not be used
	// They are now only for operations that run outside the normal decode thread, such as
	// thumbnail generation and video validation 
	PRX_INTERFACE int GetNextVideoFrame(std::shared_ptr<clVideoFrame>& outFrame);
	PRX_INTERFACE int GetNextAudioFrame(clAudioFrame& outFrame, float timeScale = 1.0f);
	PRX_INTERFACE void SeekVideo(std::shared_ptr<clVideoFrame> frame, const clTimeStamp& seekPos);
	PRX_INTERFACE void SeekVideo(const clTimeStamp& seekPos);
	PRX_INTERFACE void SeekToClosestVideoKeyFrame(std::shared_ptr<clVideoFrame> frame, const clTimeStamp& seekPos);
	PRX_INTERFACE void SeekToClosestVideoKeyFrame(std::shared_ptr<clVideoFrame> frame);

	PRX_INTERFACE clTimeStamp GetPriorVideoKeyTimeStamp(const clTimeStamp& referenceTime) const;
	PRX_INTERFACE clTimeStamp GetNextVideoKeyTimeStamp(const clTimeStamp& referenceTime) const;
	PRX_INTERFACE clTimeStamp GetClosestVideoKeyTimeStamp(const clTimeStamp& referenceTime) const;
	PRX_INTERFACE clTimeStamp GetPriorAudioKeyTimeStamp(const clTimeStamp& referenceTime) const;
	PRX_INTERFACE clTimeStamp SeekToNextKeyFrame(const clTimeStamp& seekPos);
	PRX_INTERFACE clTimeStamp SeekToPrevKeyFrame(const clTimeStamp& seekPos);
	PRX_INTERFACE void SeekToAudioFrame(const clTimeStamp& ts);

	PRX_INTERFACE clTimeStamp GetVideoDuration();
	PRX_INTERFACE clTimeStamp GetAudioDuration();
	PRX_INTERFACE clTimeStamp GetVideoFrameDuration() const;
	PRX_INTERFACE clTimeStamp GetAudioFrameDuration() const;

	PRX_INTERFACE int GetBufferSize();
	PRX_INTERFACE bool ScanKeyframeIntervalRange(int& minKeyInt, int& maxKeyInt);

	PRX_INTERFACE AVCodecID GetAudioCodecId() const;
	PRX_INTERFACE AVCodecID GetVideoCodecId() const;

	// Pulls a video frame from the given file to use as a thumbnail.  The 
	// final destination of the data depends on the virtual CopyDecodeData 
	// function in clVideoFrame
	PRX_INTERFACE static bool GenerateThumbnailFrame(const char* filename, const clTimeStamp& startPos, std::shared_ptr<clVideoFrame> thumb);
	// Pulls multiple video frames from the given file.  vecFrames.size() determines how many.
	PRX_INTERFACE static bool GenerateThumbnailFrames(const char* filename, const clTimeStamp& startPos,
		const clTimeStamp& endPos, std::vector<std::shared_ptr<clVideoFrame>>& vecFrames);
	// Open video file and try to find a "good" thumbnail
	PRX_INTERFACE static bool GenerateGoodThumbnailFrame(const char* filename, const clTimeStamp& startPos, const clTimeStamp& endPos, std::shared_ptr<clVideoFrame> thumb);


	// When exiting a project we want to clean up exiting decoding happening in the tumbnail thread
	// as quickly as possible. This flag when set short circuits decoding getting it to return as quickly as possible.
	// We don't really care if decoding was successful. Added for Patch 3.03. -
	PRX_INTERFACE static void SetDecodeShortCircuit(bool bShortCircuit) { m_bShortCircuitDecoding = bShortCircuit; }
	PRX_INTERFACE static bool GetDecodeShortCircuit() { return m_bShortCircuitDecoding; }

	PRX_INTERFACE int GetAudioSampleRate();
	PRX_INTERFACE int GetAudioDecodeFrameSize();
	PRX_INTERFACE int GetAudioNumChannels();
	PRX_INTERFACE int GetAudioSampleCount();

	PRX_INTERFACE eVideoStatus HasValidVideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration);
	PRX_INTERFACE eAudioStatus HasValidAudioAttributes(stAudioStatus& audioStatus);

	PRX_INTERFACE clTimeStamp GetCurrentVideoTimestamp();
	PRX_INTERFACE clTimeStamp GetCurrentAudioTimestamp();
	PRX_INTERFACE static void ClearYuv2(uint8_t* pOut, int nWidth, int nHeight);
	PRX_INTERFACE void PrintVideoFrameIndex();

	PRX_INTERFACE const clTimeStamp& GetMinDecodeTime() const { return m_minDecodeTime; }
	PRX_INTERFACE const clTimeStamp& GetMaxDecodeTime() const { return m_maxDecodeTime; }
	PRX_INTERFACE const clTimeStamp GetAvgDecodeTime() const { return m_sumDecodeTime / double(m_nFrameTimingCounter); }

	PRX_INTERFACE void LockStream() { m_audioDecodeMutex.Lock(); m_videoDecodeMutex.Lock(); }
	PRX_INTERFACE void UnlockStream() { m_audioDecodeMutex.Unlock(); m_videoDecodeMutex.Unlock(); }


	/*! @brief A mutex for preventing multi-threaded access to the media file open. Only one thread at a time can call av_open2() */
	static nmThread::clCriticalSectionSimple& GetCriticalSection() { return m_criticalSectionMediaFile; }
	static void InitializeCriticalSection() { m_criticalSectionMediaFile.Initialize(); };
	static void TerminateCriticalSection() { m_criticalSectionMediaFile.Terminate(); };
	static int DecodeFrame(AVCodecContext * pCodecContext, AVFrame * pFrame, int * pGotFrame, const AVPacket * pPkt);
	static int EncodePacket(AVCodecContext * pCodecContext, AVPacket * pPkt, const AVFrame * pFrame, int * pGotFrame);
	static int CodecToThreadCount(AVCodec * const codec);

private:
	eOpenFileStatus _OpenCodecContext(enum AVMediaType type, int & stream_idx, AVCodecContext *& pCodecContext);
	int _SeekFrame(int stream_index, int64_t timestamp, int flags);
	void _SeekVideoFrameByFrame(std::shared_ptr<clVideoFrame> frame, const clTimeStamp& target);
	eVideoStatus _HasValidH264VideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration);
	eVideoStatus _HasValidVP9VideoAttributes(stVideoStatus& videoStatus, const clTimeStamp& nMinDuration, const clTimeStamp& nMaxDuration);
	void _Flush();
	void _Flush(AVCodecContext* pCodecContext);
	void PrintAVError(int err, std::string from);
	AVCodecContext* GetVideoContext(bool createIfNeeded);
	void _ClearPacketQueues();
	void _ReceiveFrame(AVCodecContext *avctx, AVFrame *frame, bool& gotFrame, bool& finished);
	AVPacket* SkipToFirstKeyframePacketVideo();

private:
	AVFormatContext * m_pFormatContext;
	AVCodecContext * m_pVideoCodecContext;  // See notes above clVideoContextWrapper below
	AVCodecContext * m_pAudioCodecContext;
	AVCodecID m_nVideoCodecId;
	clTimeStamp m_reOpenSeekTime;  // If this video context had to be swapped out, seek here on reopen

	// Timing
	clTimeStamp m_sumDecodeTime;
	clTimeStamp m_minDecodeTime;
	clTimeStamp m_maxDecodeTime;
	double m_nDecodeEndTime;
	int m_nFrameTimingCounter;

	nmSystem::clFifo<AVPacket*> m_qVideoPacket;
	nmSystem::clFifo<AVPacket*> m_qAudioPacket;

	// These mutexes protect multiple threads from performing non-thread safe
	// operations on the ffmpeg stream at the same time.
	nmThread::clMutex m_audioDecodeMutex;
	nmThread::clMutex m_videoDecodeMutex;

	AVPacket m_pkt;
	AVFrame* m_frame;
	AVFrame* m_frameAudio;

	SwrContext * m_SwrContext;
	uint8_t ** m_ppnAudioDstData;

	int64_t m_nCurrentPts;
	int64_t m_nCurrentAudioPts;

	int m_nLastSampleFreq;
	int m_nVideoStreamIdx;
	int m_nAudioStreamIdx;

	bool m_bResend;
	bool m_bUseContextWrapper;
	bool m_bGotKeyframe;

	static stAudioParams m_audioParams;

	static bool m_bShortCircuitDecoding;
	static std::map<std::string, int> m_mapCodecToThreadCount;

	// A mutex for preventing multi-threaded access to the media file open
	// only one thread at a time can call av_open2()
	PRX_INTERFACE static nmThread::clCriticalSectionSimple m_criticalSectionMediaFile;
};

// NOTES:
//
// There is one instance of this class called s_videoContextWrapper in MediaFile.cpp.
// When MediaFile is constructed with useContextWrapper = true, MediaFile stores its
// video context in this wrapper instead of m_pVideoCodecContext. 
//  
// Internally, the wrapper has only N_VIDEO_CONTEXTS video contexts and a time.  Behind
// the scenes, it opens and closes these contexts as needed to hide that fact.  
//
// Only VP9 video contexts are shared here.  H264 contexts are handled elsewhere.
//
// This wrapper is not thread safe.  Videos opened from a different thread (like Thumbnails) should
// initialize clMediaFile with useContextWrapper = false.  They will then have their own independent
// video context.  
//
// 
#define N_VIDEO_CONTEXTS 6
class clVideoContextWrapper
{
public:

	clVideoContextWrapper();
	clMediaFile::eOpenFileStatus Open(clMediaFile* pMediaFile);
	void Close(clMediaFile* pMediaFile);
	AVCodecContext* GetVideoContext(clMediaFile* pMediaFile, bool createIfNeeded);

protected:

	void Close(clMediaFile* pMediaFile, AVCodecContext* pContext);

	struct stVP9PoolItem
	{
		clMediaFile* m_pMediaFile;
		AVCodecContext *m_pVideoCodecContext;
		clTimeStamp m_lastUseTime;  // last time this pool item was used
	};

	stVP9PoolItem m_aVp9Pool[N_VIDEO_CONTEXTS];
	std::map<const clMediaFile*, AVCodecContext*> m_mapH264Contexts;
};