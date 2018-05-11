#include "txWavSound.h"
#include "Utility.h"
#include "txSerializer.h"

txWavSound::txWavSound()
{
	init();
}

txWavSound::txWavSound(const std::string& file)
{
	init();
	readFile(file);
}

txWavSound::~txWavSound()
{
	txUtility::sleep(30);
	TRACE_DELETE_ARRAY(mDataBuffer);
	TRACE_DELETE(mWaveDataSerializer);
	TRACE_DELETE_ARRAY(mMixPCMData);
}

void txWavSound::init()
{
	mPlay = true;
	mIsPlaying = false;
	mFileName = EMPTY_STRING;
	mRiffMark = 0;
	mFileSize = 0;
	mWaveMark = 0;
	mFmtMark = 0;
	mFmtChunkSize = 0;
	mFormatType = 0;
	mSoundChannels = 0;
	mSamplesPerSec = 0;
	mAvgBytesPerSec = 0;
	mBlockAlign = 0;
	mBitsPerSample = 0;
	mOtherSize = 0;
	memset(mDataMark, 0, sizeof(char)* 4);
	mDataSize = 0;
	mDataBuffer = NULL;
	mMixPCMData = NULL;
	mWaveDataSerializer = NULL;
}

bool txWavSound::readFile(const std::string& file)
{
	mFileName = file;
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(file, &fileSize);
	if (buffer == NULL)
	{
		mFileName = EMPTY_STRING;
		return false;
	}
	txSerializer serializer(buffer, fileSize);
	serializer.read(mRiffMark);
	serializer.read(mFileSize);
	serializer.read(mWaveMark);
	serializer.read(mFmtMark);
	serializer.read(mFmtChunkSize);
	serializer.read(mFormatType);
	serializer.read(mSoundChannels);
	serializer.read(mSamplesPerSec);
	serializer.read(mAvgBytesPerSec);
	serializer.read(mBlockAlign);
	serializer.read(mBitsPerSample);
	if (mFmtChunkSize == 18)
	{
		serializer.read(mOtherSize);
	}
	// 如果不是data块,则跳过,重新读取
	do
	{
		TRACE_DELETE_ARRAY(mDataBuffer);
		serializer.readBuffer(mDataMark, 4, 4);
		serializer.read(mDataSize);
		mDataBuffer = TRACE_NEW_ARRAY(char, mDataSize, mDataBuffer);
		serializer.readBuffer(mDataBuffer, mDataSize, mDataSize);
	} while (!txBinaryUtility::memoryCompare(mDataMark, "data", strlen("data")));
	refreshFileSize();
	TRACE_DELETE_ARRAY(buffer);

	int mixDataCount = getMixPCMDataCount();
	mMixPCMData = TRACE_NEW_ARRAY(short, mixDataCount, mMixPCMData);
	generateMixPCMData(mMixPCMData, mixDataCount, mSoundChannels, mDataBuffer, mDataSize);
	return true;
}

void txWavSound::writeFile(const std::string& newFileName)
{
	int index = 0;
	txSerializer serializer;
	serializer.write(mRiffMark);
	serializer.write(mFileSize);
	serializer.write(mWaveMark);
	serializer.write(mFmtMark);
	serializer.write(mFmtChunkSize);
	serializer.write(mFormatType);
	serializer.write(mSoundChannels);
	serializer.write(mSamplesPerSec);
	serializer.write(mAvgBytesPerSec);
	serializer.write(mBlockAlign);
	serializer.write(mBitsPerSample);
	if (mFmtChunkSize == 18)
	{
		serializer.write(mOtherSize);
	}
	serializer.writeBuffer(mDataMark, 4);
	serializer.write(mDataSize);
	serializer.writeBuffer(mDataBuffer, mDataSize);
	txFileUtility::writeFile(newFileName, serializer.getDataSize(), serializer.getBuffer());
}

bool txWavSound::playSound()
{
	if (mIsPlaying)
	{
		return false;
	}
#if RUN_PLATFORM == PLATFORM_WINDOWS
	HANDLE threadHandle = CreateThread(NULL, 0, playThread, this, 0, NULL);
	CloseHandle(threadHandle);
#endif
	return true;
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
DWORD WINAPI txWavSound::playThread(LPVOID lpParameter)
{
	txWavSound* sound = (txWavSound*)(lpParameter);
	sound->mIsPlaying = true;
	//检查音频设备，返回音频输出设备的性能
	WAVEOUTCAPS pwoc;
	if (waveOutGetDevCaps(WAVE_MAPPER, &pwoc, sizeof(WAVEOUTCAPS)) != 0)
	{
		return 0;
	}
	//检查音频输出设备是否能播放指定的音频文件
	HWAVEOUT hWaveOut;
	WAVEFORMATEX waveFormat;
	waveFormat.nSamplesPerSec = sound->mSamplesPerSec; /* sample rate */
	waveFormat.wBitsPerSample = sound->mBitsPerSample; /* sample size */
	waveFormat.nChannels = sound->mSoundChannels; /* channels*/
	/*
	* WAVEFORMATEX also has other fields which need filling.
	* as long as the three fields above are filled this should
	* work for any PCM (pulse code modulation) format.
	*/
	waveFormat.cbSize = sound->mOtherSize; /* size of _extra_ info */
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample >> 3) * waveFormat.nChannels;
	if (waveFormat.nBlockAlign != sound->mBlockAlign)
	{
		return 0;
	}
	waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;
	if (waveFormat.nAvgBytesPerSec != sound->mAvgBytesPerSec)
	{
		return 0;
	}
	if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, NULL, NULL, CALLBACK_NULL) != 0)
	{
		return 0;
	}
	//准备待播放的数据
	WAVEHDR pWaveOutHdr;
	pWaveOutHdr.lpData = (HPSTR)sound->mDataBuffer;
	pWaveOutHdr.dwBufferLength = sound->mDataSize;
	pWaveOutHdr.dwFlags = 0;
	if (waveOutPrepareHeader(hWaveOut, &pWaveOutHdr, sizeof(WAVEHDR)) != 0)
	{
		return 0;
	}
	//播放音频数据文件
	if (waveOutWrite(hWaveOut, &pWaveOutHdr, sizeof(WAVEHDR)) != 0)
	{
		return 0;
	}
	// 每隔30毫秒判断是否还在播放
	while (sound->mPlay && waveOutUnprepareHeader(hWaveOut, &pWaveOutHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING)
	{
		txUtility::sleep(30);
	}
	//关闭音频输出设备,释放内存
	waveOutReset(hWaveOut);
	waveOutClose(hWaveOut);
	sound->mIsPlaying = false;
	return 0;
}
#endif

void txWavSound::generateMixPCMData(short* mixPCMData, const int& mixDataCount, const int& channelCount, const char* dataBuffer, const int& bufferSize)
{
	// 如果单声道,则直接将mDataBuffer的数据拷贝到mMixPCMData中
	if (channelCount == 1)
	{
		memcpy(mixPCMData, dataBuffer, bufferSize);
	}
	// 如果有两个声道,则将左右两个声道的平均值赋值到mMixPCMData中
	else if (channelCount == 2)
	{
		short* shortData = (short*)dataBuffer;
		for (int i = 0; i < mixDataCount; ++i)
		{
			mixPCMData[i] = (shortData[2 * i + 0] + shortData[2 * i + 1]) / 2;
		}
	}
}

void txWavSound::refreshFileSize()
{
	// 由于舍弃了fact块,所以需要重新计算文件大小,20是fmt块数据区的起始偏移,8是data块的头的大小
	mFileSize = 20 - 8 + mFmtChunkSize + 8 + mDataSize;
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
void txWavSound::startWaveStream(const WAVEFORMATEX& waveHeader)
{
	TRACE_DELETE(mWaveDataSerializer);
	mWaveDataSerializer = TRACE_NEW(txSerializer, mWaveDataSerializer);
	memcpy(&mRiffMark, "RIFF", 4);
	mFileSize = 0;
	memcpy(&mWaveMark, "WAVE", 4);
	memcpy(&mFmtMark, "fmt ", 4);
	mFmtChunkSize = 16;
	mFormatType = waveHeader.wFormatTag;
	mSoundChannels = waveHeader.nChannels;
	mSamplesPerSec = waveHeader.nSamplesPerSec;
	mAvgBytesPerSec = waveHeader.nAvgBytesPerSec;
	mBlockAlign = waveHeader.nBlockAlign;
	mBitsPerSample = waveHeader.wBitsPerSample;
	mOtherSize = waveHeader.cbSize;
	memcpy(mDataMark, "data", 4);
}
#endif

void txWavSound::pushWaveStream(char* data, const int& dataSize)
{
	mWaveDataSerializer->writeBuffer(data, dataSize);
}

void txWavSound::endWaveStream()
{
	mDataSize = mWaveDataSerializer->getDataSize();
	TRACE_DELETE_ARRAY(mDataBuffer);
	mDataBuffer = TRACE_NEW_ARRAY(char, mDataSize, mDataBuffer);
	memcpy(mDataBuffer, mWaveDataSerializer->getBuffer(), mDataSize);
	TRACE_DELETE(mWaveDataSerializer);
	int mixDataCount = getMixPCMDataCount();
	mMixPCMData = TRACE_NEW_ARRAY(short, mixDataCount, mMixPCMData);
	generateMixPCMData(mMixPCMData, mixDataCount, mSoundChannels, mDataBuffer, mDataSize);
	refreshFileSize();
}