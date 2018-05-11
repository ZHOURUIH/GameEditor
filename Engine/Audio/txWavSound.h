#ifndef _TX_WAV_SOUND_H_
#define _TX_WAV_SOUND_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txSerializer;
class txWavSound : public txEngineBase
{
public:
	txWavSound();
	txWavSound(const std::string& file);
	virtual ~txWavSound();
	void init();
	bool readFile(const std::string& file);
	void writeFile(const std::string& newFileName);
	const char* getPCMBuffer(){ return mDataBuffer; }
	const short* getShortPCMData() { return (short*)(mDataBuffer); }
	const short* getMixPCMData() { return mMixPCMData; }
	const DWORD& getPCMBufferSize() { return mDataSize; }
	const WORD& getSoundChannels() { return mSoundChannels; }
	DWORD getPCMShortDataCount() { return mDataSize / sizeof(short); }
	DWORD getMixPCMDataCount() { return mDataSize / sizeof(short) / mSoundChannels; }
	bool playSound();
#if RUN_PLATFORM == PLATFORM_WINDOWS
	void startWaveStream(const WAVEFORMATEX& waveHeader);
#endif
	void pushWaveStream(char* data, const int& dataSize);
	void endWaveStream();
	static void generateMixPCMData(short* mixPCMData, const int& mixDataCount, const int& channelCount, const char* dataBuffer, const int& bufferSize);
protected:
#if RUN_PLATFORM == PLATFORM_WINDOWS
	static DWORD WINAPI playThread(LPVOID lpParameter);
#endif
	void refreshFileSize();
public:
	std::string mFileName;
	DWORD mRiffMark;			// riff标记
	DWORD mFileSize;			// 音频文件大小 - 8,也就是从文件大小字节后到文件结尾的长度
	DWORD mWaveMark;			// wave标记
	DWORD mFmtMark;				// fmt 标记
	DWORD mFmtChunkSize;		// fmt块大小
	WORD mFormatType;			// 编码格式,为1是PCM编码
	WORD mSoundChannels;		// 声道数
	DWORD mSamplesPerSec;		// 采样频率
	DWORD mAvgBytesPerSec;		// 波形数据传输速率（每秒平均字节数）
	WORD mBlockAlign;			// DATA数据块长度
	WORD mBitsPerSample;		// 单个采样数据大小,如果双声道16位,则是4个字节,也叫PCM位宽
	WORD mOtherSize;			// 附加信息（可选，由上方过滤字节确定）
	char mDataMark[4];			// data标记
	DWORD mDataSize;
	char* mDataBuffer;
	short* mMixPCMData;
	volatile bool mPlay;
	volatile bool mIsPlaying;
	txSerializer* mWaveDataSerializer;
};

#endif