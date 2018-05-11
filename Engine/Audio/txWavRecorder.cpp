#include "txWavRecorder.h"
#include "txWavSound.h"
#include "Utility.h"

txWavRecorder::txWavRecorder(const int& bufferSize, const int& sampleRate, const int& channelCount)
:
mBufferSize(bufferSize),
mSampleRate(sampleRate),
mChannelCount(channelCount),
mBitsPerSample(16),
mRecordCallback(NULL),
mUserData(NULL)
{
	mSound = TRACE_NEW(txWavSound, mSound);
	mWaveDataHeader0.dwBufferLength = mBufferSize;
	mWaveDataHeader0.lpData = TRACE_NEW_ARRAY(char, mWaveDataHeader0.dwBufferLength, mWaveDataHeader0.lpData);
	mWaveDataHeader1.dwBufferLength = mBufferSize;
	mWaveDataHeader1.lpData = TRACE_NEW_ARRAY(char, mWaveDataHeader1.dwBufferLength, mWaveDataHeader1.lpData);
}

txWavRecorder::~txWavRecorder()
{
	TRACE_DELETE(mSound);
	TRACE_DELETE_ARRAY(mWaveDataHeader0.lpData);
	TRACE_DELETE_ARRAY(mWaveDataHeader1.lpData);
}

bool txWavRecorder::startRecord(const bool& saveRecordData)
{
	TRACE_DELETE(mSound);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	// 音频输入数量
	int count = waveInGetNumDevs();
	if (count <= 0)
	{
		return false;
	}

	WAVEINCAPS waveIncaps;
	// 音频输入设备
	MMRESULT mmResult = waveInGetDevCaps(0, &waveIncaps, sizeof(WAVEINCAPS));
	if (MMSYSERR_NOERROR != mmResult)
	{
		return false;
	}

	// 采样参数
	WAVEFORMATEX pwfx;
	pwfx.wFormatTag = WAVE_FORMAT_PCM;
	pwfx.nChannels = mChannelCount;
	pwfx.nSamplesPerSec = mSampleRate;
	pwfx.wBitsPerSample = mBitsPerSample;
	pwfx.nAvgBytesPerSec = pwfx.nSamplesPerSec * pwfx.nChannels * pwfx.wBitsPerSample / 8;
	pwfx.nBlockAlign = pwfx.nChannels * pwfx.wBitsPerSample / 8;
	pwfx.cbSize = 0;

	// 创建保存的声音
	if (saveRecordData)
	{
		mSound = TRACE_NEW(txWavSound, mSound);
		mSound->startWaveStream(pwfx);
	}
	// 请求打开音频输入设备
	mmResult = waveInOpen(&mWaveInHandle, WAVE_MAPPER, &pwfx, (DWORD_PTR)(micCallback), (DWORD_PTR)this, CALLBACK_FUNCTION);

	if (MMSYSERR_NOERROR != mmResult)
	{
		return false;
	}
	
	mWaveDataHeader0.dwUser = 1;
	mWaveDataHeader0.dwFlags = 0;
	mmResult = waveInPrepareHeader(mWaveInHandle, &mWaveDataHeader0, sizeof(WAVEHDR));
	mWaveDataHeader1.dwUser = 2;
	mWaveDataHeader1.dwFlags = 0;
	mmResult = waveInPrepareHeader(mWaveInHandle, &mWaveDataHeader1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != mmResult)
	{
		return false;
	}
	mmResult = waveInAddBuffer(mWaveInHandle, &mWaveDataHeader0, sizeof(WAVEHDR));
	mmResult = waveInAddBuffer(mWaveInHandle, &mWaveDataHeader1, sizeof(WAVEHDR));
	if (MMSYSERR_NOERROR != mmResult)
	{
		return false;
	}
	// 请求开始录音
	mmResult = waveInStart(mWaveInHandle);
#endif
	return true;
}

void txWavRecorder::stopRecord()
{
#if RUN_PLATFORM == PLATFORM_WINDOWS
	while (waveInUnprepareHeader(mWaveInHandle, &mWaveDataHeader0, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING || waveInUnprepareHeader(mWaveInHandle, &mWaveDataHeader1, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING){}
	waveInClose(mWaveInHandle);
#endif
	if (mSound != NULL)
	{
		mSound->endWaveStream();
	}
}

void txWavRecorder::notifyReceiveWavData(char* data, const int& dataSize)
{
	if (mSound != NULL)
	{
		mSound->pushWaveStream(data, dataSize);
	}
	if (mRecordCallback != NULL)
	{
		mRecordCallback(this, mUserData, data, dataSize);
	}
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
DWORD CALLBACK txWavRecorder::micCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	txWavRecorder* recorder = (txWavRecorder*)(dwInstance);
	if (uMsg == WIM_DATA)
	{
		// 将缓冲区中的数据取出并保存
		LPWAVEHDR waveHeader = (LPWAVEHDR)dwParam1;
		recorder->notifyReceiveWavData(waveHeader->lpData, waveHeader->dwBufferLength);
		waveInAddBuffer(hwavein, (LPWAVEHDR)dwParam1, sizeof (WAVEHDR));
	}
	return 0;
}
#endif