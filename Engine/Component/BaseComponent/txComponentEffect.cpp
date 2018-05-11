#include "txEngineRoot.h"
#include "txEntity.h"
#include "txMagicParticleSystem.h"

#include "txComponentHeader.h"
#include "txObjectBoneHook.h"
#include "txEffectProperty.h"

void txComponentEffect::initProperty()
{
	txComponent::initProperty();
	addProperty<txEffectPropertyParticle>();

	txEffectPropertyParticle* magicParticlProperty = getProperty<txEffectPropertyParticle>();
	if (magicParticlProperty->getChoices() != NULL && magicParticlProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		txFileUtility::findFiles(P_PARTICLE_PATH, fileList, ".ptc");
		txVector<std::string> fileOnlyList;
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			fileOnlyList.push_back(txStringUtility::getFileName(fileList[i]));
		}
		END_FOR_STL(fileList);
		magicParticlProperty->setChoices(fileOnlyList);
	}
}

void txComponentEffect::destroy()
{
	TRACE_DELETE(mHook);
}

void txComponentEffect::update(float elapsedTime)
{
	// 更新挂点
	if (mHook != NULL)
	{
		const MATRIX4& hookWorldTransform = mHook->getWorldTransform();
		auto& emitters = mMagicParticleSystem->GetAllEmittersFromFile(mEffectFileName);
		auto iter = emitters.begin();
		auto iterEnd = emitters.end();
		FOR_STL(emitters, ; iter != iterEnd; ++iter)
		{
			mMagicParticleSystem->GetEmitter(iter->second)->GetParent()->SetLocalTransform(hookWorldTransform);
		}
		END_FOR_STL(emitters);
	}
	txComponent::update(elapsedTime);
}

void txComponentEffect::setRotate(const MATRIX3& rotate)
{
	if (mHook != NULL)
	{
		mHook->setRotation(rotate);
	}
	else
	{
		auto& emitterHanndleList = mMagicParticleSystem->GetAllEmittersFromFile(mEffectFileName);
		auto iterEmitter = emitterHanndleList.begin();
		auto iterEmitterEnd = emitterHanndleList.end();
		FOR_STL(emitterHanndleList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mMagicParticleSystem->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				txNode* emitterNode = emitter->GetParent();
				emitterNode->SetRotationMatrix(rotate);
			}
		}
		END_FOR_STL(emitterHanndleList);
	}
}

void txComponentEffect::setPosition(const VECTOR3& translate)
{
	if (mHook != NULL)
	{
		mHook->setPosition(translate);
	}
	else
	{
		auto& emitterHanndleList = mMagicParticleSystem->GetAllEmittersFromFile(mEffectFileName);
		auto iterEmitter = emitterHanndleList.begin();
		auto iterEmitterEnd = emitterHanndleList.end();
		FOR_STL(emitterHanndleList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mMagicParticleSystem->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				txNode* emitterNode = emitter->GetParent();
				emitterNode->SetPosition(translate);
			}
		}
		END_FOR_STL(emitterHanndleList);
	}
}

void txComponentEffect::setScale(const VECTOR3& scale)
{
	if (mHook != NULL)
	{
		mHook->setScale(scale);
	}
	else
	{
		auto& emitterHanndleList = mMagicParticleSystem->GetAllEmittersFromFile(mEffectFileName);
		auto iterEmitter = emitterHanndleList.begin();
		auto iterEmitterEnd = emitterHanndleList.end();
		FOR_STL(emitterHanndleList, ; iterEmitter != iterEmitterEnd; ++iterEmitter)
		{
			MP_Emitter* emitter = mMagicParticleSystem->GetEmitter(iterEmitter->second);
			if (emitter != NULL)
			{
				txNode* emitterNode = emitter->GetParent();
				emitterNode->SetScale(scale);
			}
		}
		END_FOR_STL(emitterHanndleList);
	}
}

bool txComponentEffect::createHook(const std::string& boneName, txEntity* hookEntity)
{
	if (mHook != NULL)
	{
		return false;
	}
	mHook = TRACE_NEW(txObjectBoneHook, mHook, mName);
	if (hookEntity != NULL)
	{
		mHook->createHook(boneName, hookEntity);
	}
	return false;
}

bool txComponentEffect::createEffect(const std::string& fileName, txNode* parentNode)
{
	mEffectFileName = fileName;
	if (mHook == NULL)
	{
		mMagicParticleSystem->LoadEmitter(mEffectFileName.c_str(), parentNode);
	}
	else
	{
		mMagicParticleSystem->LoadEmitter(mEffectFileName.c_str(), mHook->getEntity()->GetParent());
	}
	return true;
}

void txComponentEffect::SetEmittersUpdateMode(const bool& loop)
{
	mMagicParticleSystem->SetEmitterLoopMode(mEffectFileName, loop ? EUM_LOOP : EUM_ONCE);
}

void txComponentEffect::play()
{
	mMagicParticleSystem->PlayEmitter(mEffectFileName);
}

void txComponentEffect::stop()
{
	mMagicParticleSystem->StopEmitter(mEffectFileName);
}
