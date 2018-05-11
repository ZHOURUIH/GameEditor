#ifndef _TX_WAV_RECORDER_H_
#define _TX_WAV_RECORDER_H_

#include "txEngineDefine.h"

class txWavSound;
class txWavRecorder
{
public:
	txWavRecorder(const int& bufferSize, const int& sampleRate = 44100, const int& channelCount = 2);
	virtual ~txWavRecorder();
	bool startRecord(const bool& saveRecordData = true);
	void stopRecord();
	void setRecordCallback(RecordCallback function, void* userData)
	{
		mRecordCallback = function;
		mUserData = userData;
	}
	const int& getSampleRate() { return mSampleRate; }
	const int& getChannelCount() { return mChannelCount; }
	const int& getBitsPerSample() { return mBitsPerSample; }
	const int& getBufferSize() { return mBufferSize; }
	txWavSound* getWavSound() { return mSound; }
protected:
	void notifyReceiveWavData(char* data, const int& dataSize);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD CALLBACK micCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
#endif
protected:
	txWavSound* mSound;
	RecordCallback mRecordCallback;
	void* mUserData;
	int mSampleRate;				// 音频采样率
	int mChannelCount;				// 采样声道数
	int mBitsPerSample;				// 每个采样单位的大小,位
#if RUN_PLATFORM == PLATFORM_WINDOWS
	HWAVEIN mWaveInHandle;
	WAVEHDR mWaveDataHeader0;
	WAVEHDR mWaveDataHeader1;
#endif
	const int mBufferSize;
};

#endif