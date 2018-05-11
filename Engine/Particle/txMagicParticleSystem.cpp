#include "txMagicParticleSystem.h"
#if RUN_PLATFORM == PLATFORM_ANDROID
#include "txPlatform_android.h"
#elif RUN_PLATFORM == PLATFORM_WINDOWS
#include "txPlatform_win_posix.h"
#endif
#include "txMP_Device.h"
#include "txNode.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"

txMap<std::string, HM_EMITTER> txMagicParticleSystem::EMPTY_EMITTER_LIST;

txMagicParticleSystem::txMagicParticleSystem()
{
	mDevice = NULL;
	mParticleManager = NULL;
	mParticleFileList.clear();
}

txMagicParticleSystem::~txMagicParticleSystem()
{
	StopAllEmitter();
	TRACE_DELETE(mDevice);
	TRACE_DELETE(mParticleManager);
	mParticleFileList.clear();
}

void txMagicParticleSystem::init()
{
	mDevice = TRACE_NEW(txMP_Device, mDevice);
	mDevice->Create();

	mParticleManager = TRACE_NEW(MP_Manager, mParticleManager);

	#ifdef MAGIC_3D
	int axis = MAGIC_pXpYpZ;
	#else
	int axis = MAGIC_pXnY;
	#endif
#if RUN_PLATFORM == PLATFORM_ANDROID
	MP_Platform* platform = TRACE_NEW (MP_Platform_ANDROID, platform);
#elif  RUN_PLATFORM == PLATFORM_WINDOWS
	MP_Platform* platform = TRACE_NEW(MP_Platform_WIN_POSIX, platform);
#endif
	mParticleManager->Initialization(axis, platform, 128, 128, 1, 1.0f, 0.1f, true);

	mParticleManager->Stop();

	#ifndef MAGIC_3D
	// eng: locate emitters the same as editor
	HM_EMITTER hmEmitter = MP.GetFirstEmitter();
	while (hmEmitter)
	{
		Magic_CorrectEmitterPosition(hmEmitter, width, height);
		hmEmitter = MP.GetNextEmitter(hmEmitter);
	}
	#endif
}

void txMagicParticleSystem::LoadAllEmitters()
{
	mParticleFileList.clear();
	mParticleManager->LoadAllEmitters(mParticleFileList);
	mParticleManager->CloseFiles();
}

void txMagicParticleSystem::LoadEmitter(const char* ptcFile, txNode* parent, const bool& changeParticleByEmitter, const bool& interpolation)
{
	if(mParticleFileList.find(ptcFile) != mParticleFileList.end())
	{
		ENGINE_ERROR("error : ptcFile named : %s have already loaded!", ptcFile);
		return;
	}

	HM_FILE hFile;
	std::pair<std::string, PtcStruct> particleFilePair;
	if(!mParticleManager->LoadEmittersFromFile(ptcFile, parent, &hFile, particleFilePair))
	{
		ENGINE_ERROR("error : load particle file named : %s failed.", ptcFile);
		return;
	}

	// 加载完后就将文件关闭
	int ret = mParticleManager->CloseFile(hFile);
	if(ret != MAGIC_SUCCESS)
	{
		ENGINE_ERROR("error : failed to close ptc file");
	}

	mParticleFileList.insert(particleFilePair.first, particleFilePair.second);
	auto iter = particleFilePair.second.mEmitterList.begin();
	auto iterEnd = particleFilePair.second.mEmitterList.end();
	FOR_STL(particleFilePair.second.mEmitterList, ; iter != iterEnd; ++iter)
	{
		MP_Emitter* emitter = mParticleManager->GetEmitter(iter->second);
		emitter->SetState(ES_UPDATE);
		emitter->SetUpdateMode(EUM_LOOP);
		emitter->SetInterpolation(interpolation);
		txNode* pNode = emitter->GetParent();
		if(pNode != NULL)
		{
			pNode->ResetRotation();
			pNode->SetAlwaysUpdate(true);
		}
		if(changeParticleByEmitter)
		{
			emitter->SetMagicChangeMode(MCM_EMITTER_AND_PARTICLES);
		}
		else
		{
			emitter->SetMagicChangeMode(MCM_EMITTER_ONLY);
		}
	}
	END_FOR_STL(particleFilePair.second.mEmitterList);
}

// 根据一个已经加载的粒子文件,创建相同的发射器
std::string txMagicParticleSystem::LoadEmitterByLoadedFile(const char* ptcFile, txNode* parent, const bool& changeParticleByEmitter, const bool& interpolation)
{
	auto iter = mParticleFileList.find(ptcFile);
	if(iter == mParticleFileList.end())
	{
		ENGINE_ERROR("error : ptcFile named %s is not loaded! can not duplicate it!", ptcFile);
		return EMPTY_STRING;
	}

	PtcStruct ptcStruct;
	auto iterEmitter = iter->second.mEmitterList.begin();
	auto iterEmitterEnd = iter->second.mEmitterList.end();
	FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
	{
		HM_EMITTER hEmitter = mParticleManager->DuplicateEmitter(iterEmitter->second);
		ptcStruct.mEmitterList.insert(iterEmitter->first, hEmitter);

		MP_Emitter* pEmitter = GetEmitter(hEmitter);
		if(pEmitter != NULL)
		{
			pEmitter->SetParentNode(parent);
		}
	}
	END_FOR_STL(iter->second.mEmitterList);
	char ptcNameBuffer[256];
	SPRINTF(ptcNameBuffer, 256, "%s_copy%d", ptcFile, iter->second.mCpoySeed);
	++iter->second.mCpoySeed;

	auto iterEmi = ptcStruct.mEmitterList.begin();
	auto iterEmiEnd = ptcStruct.mEmitterList.end();
	FOR_STL(ptcStruct.mEmitterList,; iterEmi != iterEmiEnd; ++iterEmi)
	{
		MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmi->second);
		emitter->SetState(ES_UPDATE);
		emitter->SetUpdateMode(EUM_LOOP);
		emitter->SetInterpolation(interpolation);
		txNode* pNode = emitter->GetParent();
		if(pNode != NULL)
		{
			pNode->ResetRotation();
		}
		if(changeParticleByEmitter)
		{
			emitter->SetMagicChangeMode(MCM_EMITTER_AND_PARTICLES);
		}
		else
		{
			emitter->SetMagicChangeMode(MCM_EMITTER_ONLY);
		}
	}
	END_FOR_STL(ptcStruct.mEmitterList);

	mParticleFileList.insert(ptcNameBuffer, ptcStruct);
	return ptcNameBuffer;
}

// 卸载发射器,会将发射器从内存中销毁掉
void txMagicParticleSystem::UnLoadEmitter(const HM_EMITTER& hEmitter)
{
	int ret = mParticleManager->DeleteEmitter(hEmitter);
	if(ret != MAGIC_SUCCESS)
	{
		ENGINE_ERROR("error : failed to unload emitter");
	}
}

txMap<std::string, HM_EMITTER>& txMagicParticleSystem::GetAllEmittersFromFile(const std::string& ptcFile)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		return iter->second.mEmitterList;
	}
	return EMPTY_EMITTER_LIST;
}

// 设置一个粒子文件中所有发射器的父节点
void txMagicParticleSystem::SetEmitterParentNode(const std::string& ptcFile, txNode* parent)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		auto iterEmitter = iter->second.mEmitterList.begin();
		auto iterEmitterEnd = iter->second.mEmitterList.end();
		FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmitter->second);
			emitter->SetParentNode(parent);
		}
		END_FOR_STL(iter->second.mEmitterList);
	}
}

// 设置发射器的父节点
void txMagicParticleSystem::SetEmitterParentNode(const HM_EMITTER& hEmitter, txNode* parent)
{
	MP_Emitter* emitter = mParticleManager->GetEmitter(hEmitter);
	emitter->SetParentNode(parent);
}

// 停止全部的发射器
void txMagicParticleSystem::StopAllEmitter()
{
	mParticleManager->Stop();
}

// 设置一个粒子文件中所有发射器的状态
bool txMagicParticleSystem::SetEmitterState(const std::string& ptcFile, const EMITTER_STATE& state)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		bool ret = true;
		auto iterEmitter = iter->second.mEmitterList.begin();
		auto iterEmitterEnd = iter->second.mEmitterList.end();
		FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				emitter->SetState(state);
			}
			else
			{
				ret = false;
				break;
			}
		}
		END_FOR_STL(iter->second.mEmitterList);
		return ret;
	}
	return false;
}

// 设置发射器状态
bool txMagicParticleSystem::SetEmitterState(const HM_EMITTER& hEmitter, const EMITTER_STATE& state)
{
	MP_Emitter* emitter = mParticleManager->GetEmitter(hEmitter);
	if(emitter != NULL)
	{
		emitter->SetState(state);
	}
	else
	{
		return false;
	}
	return true;
}

// 设置一个粒子文件中所有发射器循环状态
bool txMagicParticleSystem::SetEmitterLoopMode(const std::string& ptcFile, const EMITTER_UPDATE_MODE& loop)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		bool ret = true;
		auto iterEmitter = iter->second.mEmitterList.begin();
		auto iterEmitterEnd = iter->second.mEmitterList.end();
		FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				emitter->SetUpdateMode(loop);
			}
			else
			{
				ret = false;
				break;
			}
		}
		END_FOR_STL(iter->second.mEmitterList);
		return ret;
	}
	return false;
}

// 设置发射器循环状态
bool txMagicParticleSystem::SetEmitterLoopMode(const HM_EMITTER& hEmitter, const EMITTER_UPDATE_MODE& loop)
{
	MP_Emitter* emitter = mParticleManager->GetEmitter(hEmitter);
	if (emitter != NULL)
	{
		emitter->SetUpdateMode(loop);
	}
	else
	{
		return false;
	}
	return true;
}

// 播放一个粒子文件中所有的发射器
void txMagicParticleSystem::PlayEmitter(const std::string& ptcFile)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		auto iterEmitter = iter->second.mEmitterList.begin();
		auto iterEmitterEnd = iter->second.mEmitterList.end();
		FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				emitter->SetState(ES_UPDATE);
			}
		}
		END_FOR_STL(iter->second.mEmitterList);
	}
}

// 播放发射器
void txMagicParticleSystem::PlayEmitter(const HM_EMITTER& hEmitter)
{
	MP_Emitter* emitter = mParticleManager->GetEmitter(hEmitter);
	if(emitter != NULL)
	{
		emitter->SetState(ES_UPDATE);
	}
}

// 停止一个粒子文件中所有的发射器
void txMagicParticleSystem::StopEmitter(const std::string& ptcFile)
{
	auto iter = mParticleFileList.find(ptcFile);
	if (iter != mParticleFileList.end())
	{
		auto iterEmitter = iter->second.mEmitterList.begin();
		auto iterEmitterEnd = iter->second.mEmitterList.end();
		FOR_STL(iter->second.mEmitterList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mParticleManager->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				emitter->SetState(ES_STOP);
			}
		}
		END_FOR_STL(iter->second.mEmitterList);
	}
}

// 停止发射器
void txMagicParticleSystem::StopEmitter(const HM_EMITTER& hEmitter)
{
	MP_Emitter* emitter = mParticleManager->GetEmitter(hEmitter);
	if(emitter != NULL)
	{
		emitter->SetState(ES_STOP);
	}
}

// 得到发射器
MP_Emitter* txMagicParticleSystem::GetEmitter(const HM_EMITTER& hEmitter)
{
	return mParticleManager->GetEmitter(hEmitter);
}

MP_Emitter* txMagicParticleSystem::GetEmitterPointerByName(const std::string& ptcFile, const std::string& emitterName)
{
	return GetEmitter(GetEmitterHandleByName(ptcFile, emitterName));
}

HM_EMITTER txMagicParticleSystem::GetEmitterHandleByName(const std::string& ptcFile, const std::string& emitterName)
{
	auto iterFile = mParticleFileList.find(ptcFile);
	if(iterFile == mParticleFileList.end())
	{
		ENGINE_ERROR("error : can not find ptcFile named %s", ptcFile.c_str());
		return -1;
	}
	auto iterEmitter = iterFile->second.mEmitterList.find(emitterName);
	if(iterEmitter == iterFile->second.mEmitterList.end())
	{
		ENGINE_ERROR("error : can not find emitter %s in file %s", emitterName.c_str(), ptcFile.c_str());
		return -1;
	}
	return iterEmitter->second;
}

void txMagicParticleSystem::RenderParticles(txCamera* camera)
{
	mParticleManager->Render(camera);
}

