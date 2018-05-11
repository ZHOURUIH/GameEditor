#include "txRendererManager.h"
#include "txBillboard.h"
#include "txSceneManager.h"
#include "txSceneManager.h"
#include "txNode.h"
#include "txCamera.h"
#include "txTexture.h"
#include "txBillboardManager.h"
#include "txNodeManager.h"
#include "txTextureManager.h"
#include "txRenderer_Texture.h"

txRendererTexture* txBillboard::mTextureRenderer = NULL;

txBillboard::txBillboard(const std::string& name)
:
txObject(OT_BILLBOARD),
mWidth(0.0f),
mHeight(0.0f),
mDuration(1.0f),
mCurTexture(NULL),
mCamera(NULL),
mCurPlayTime(0.0f),
mCurTextureIndex(0),
mAutoDestroy(false),
mVertices(NULL),
mTexCoord(NULL),
mIndices(NULL),
mBillboardState(PS_STOP),
mAlpha(1.0f),
mColour(txMath::VEC3_ZERO),
mColourBlendMode(BM_ADD),
mVisible(false),
mHideAndStopPlayEnd(false)
{
	setName(name.c_str());

	if (mTextureRenderer == NULL)
	{
		mTextureRenderer = mRendererManager->getInstance<txRendererTexture>();
	}
}

txBillboard::~txBillboard()
{
	mParent->DetachObject();
	if(mParent != NULL)
	{
		mNodeManager->DestroyNode(mParent->GetName());
		mParent = NULL;
	}
	mWidth = 0.0f;
	mHeight = 0.0f;
	mCurTexture = NULL;
	mCurPlayTime = 0.0f;
	mCurTextureIndex = 0;
	mAutoDestroy = false;
	mCamera = NULL;
	destroyVertex();
	destroyTexture();
}

void txBillboard::init(txCamera* camera)
{
	mCamera = camera;
	mWidth = 0.0f;
	mHeight = 0.0f;
	mDuration = 1.0f;
	mCurTexture = NULL;
	mCamera = NULL;
	mCurPlayTime = 0.0f;
	mCurTextureIndex = 0;
	mAutoDestroy = false;
	mVertices = NULL;
	mTexCoord = NULL;
	mIndices = NULL;
	mBillboardState = PS_STOP;
	mVisible = false;
	mHideAndStopPlayEnd = false;
	destroyVertex();
	destroyTexture();
}

void txBillboard::destroyTexture()
{
	int listSize = mTextureNameList.size();
	FOR_STL(mTextureNameList, int i = 0; i < listSize; ++i)
	{
		mTextureManager->destroyTexture(mTextureNameList[i]);
	}
	END_FOR_STL(mTextureNameList);
	mTextureNameList.clear();
}

void txBillboard::destroyVertex()
{
	TRACE_DELETE_ARRAY(mVertices);
	TRACE_DELETE_ARRAY(mTexCoord);
	TRACE_DELETE_ARRAY(mIndices);
}

void txBillboard::createVertex()
{
	if(mVertices != NULL || mTexCoord != NULL || mIndices != NULL)
	{
		destroyVertex();
	}
	// 顶点坐标
	mVertices = TRACE_NEW_ARRAY(GLfloat, 4 * 3, mVertices);
	// 第一个顶点
	int vertexIndex = 0;
	VECTOR3 pos0(-mWidth, -mHeight, 0.0f);
	mVertices[3 * vertexIndex + 0] = pos0.x;
	mVertices[3 * vertexIndex + 1] = pos0.y;
	mVertices[3 * vertexIndex + 2] = pos0.z;
	// 第二个顶点
	++vertexIndex;
	VECTOR3 pos1(mWidth, -mHeight, 0.0f);
	mVertices[3 * vertexIndex + 0] = pos1.x;
	mVertices[3 * vertexIndex + 1] = pos1.y;
	mVertices[3 * vertexIndex + 2] = pos1.z;
	// 第三个顶点
	++vertexIndex;
	VECTOR3 pos2(mWidth, mHeight, 0.0f);
	mVertices[3 * vertexIndex + 0] = pos2.x;
	mVertices[3 * vertexIndex + 1] = pos2.y;
	mVertices[3 * vertexIndex + 2] = pos2.z;
	// 第四个顶点
	++vertexIndex;
	VECTOR3 pos3(-mWidth, mHeight, 0.0f);
	mVertices[3 * vertexIndex + 0] = pos3.x;
	mVertices[3 * vertexIndex + 1] = pos3.y;
	mVertices[3 * vertexIndex + 2] = pos3.z;

	// 纹理坐标
	mTexCoord = TRACE_NEW_ARRAY(GLfloat, 4 * 2, mTexCoord);
	// 第一个纹理坐标
	int textureCoordIndex = 0;
	mTexCoord[2 * textureCoordIndex + 0] = 0.0f;
	mTexCoord[2 * textureCoordIndex + 1] = 0.0f;

	// 第二个纹理坐标
	++textureCoordIndex;
	mTexCoord[2 * textureCoordIndex + 0] = 1.0f;
	mTexCoord[2 * textureCoordIndex + 1] = 0.0f;

	// 第三个纹理坐标
	++textureCoordIndex;
	mTexCoord[2 * textureCoordIndex + 0] = 1.0f;
	mTexCoord[2 * textureCoordIndex + 1] = 1.0f;

	// 第四个纹理坐标
	++textureCoordIndex;
	mTexCoord[2 * textureCoordIndex + 0] = 0.0f;
	mTexCoord[2 * textureCoordIndex + 1] = 1.0f;

	// 两个三角形的顶点索引
	mIndices = TRACE_NEW_ARRAY(GLushort, 6, mIndices);
	int triangleIndex = 0;
	// 第一个三角形
	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 1;
	mIndices[triangleIndex * 3 + 2] = 2;
	// 第二个三角形
	++triangleIndex;
	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 2;
	mIndices[triangleIndex * 3 + 2] = 3;
}

// 设置序列帧图片
void txBillboard::setTextureName(const std::string& textureName, const int& frameCount)
{
	destroyTexture();
	if(frameCount == 1)
	{
		txTexture* pTexture = mTextureManager->createTexture(textureName);
		pTexture->initTexture();
		mTextureNameList.push_back(textureName);
	}
	else
	{
		int dotPos = textureName.find_last_of('.');
		std::string preName = textureName.substr(0, dotPos);
		std::string exName = textureName.substr(dotPos, textureName.length() - dotPos);
		char name[1024];
		for(int i = 0; i < frameCount; ++i)
		{
			SPRINTF(name, 1024, "%s%d%s", preName.c_str(), i, exName.c_str());
			txTexture* pTexture = mTextureManager->createTexture(name);
			pTexture->initTexture();
			mTextureNameList.push_back(name);
		}
	}
}

// 设置父节点,只在设置父节点时才创建公告板的节点,并且把公告板挂接到节点上,再把公告板的节点挂接到设置的父节点上
// 注意,该函数不能在节点的更新函数中调用,否则父节点在更新自己的子节点时会发生迭代器失效
void txBillboard::setParentNode(txNode* parentNode)
{
	// 如果节点为空,则创建节点,并且将公告板挂接到节点上
	if(mParent == NULL)
	{
		mParent = mNodeManager->CreateNode();
		mParent->AttachObject(this);
	}
	// 如果节点不为空,并且有父节点,则从父节点上取下该节点
	else
	{
		if(mParent->GetParent() != NULL)
		{
			mParent->GetParent()->RemoveChildNode(mParent);
		}
	}
	if(parentNode != NULL)
	{
		parentNode->AddChildNode(mParent);
	}
}

void txBillboard::stop(const bool& visible)
{
	mBillboardState = PS_STOP;
	mCurTextureIndex = 0;
	mCurTexture = mTextureManager->getTexture(mTextureNameList[mCurTextureIndex]);
	mCurPlayTime = 0.0f;
	setVisible(visible);
}

void txBillboard::Update(float fElaspedTime)
{
	if(mParent == NULL || mCamera == NULL)
	{
		return;
	}
	// 只有处于播放状态才更新序列帧和朝向
	if(mBillboardState == PS_PLAY)
	{
		// 计算视图矩阵的逆矩阵,然后将位移设置为0,作为公告板的变换矩阵
		MATRIX3 viewMatrix = txMath::getMatrixRotation(mCamera->getViewMatrix());
		viewMatrix = txMath::inverse(viewMatrix);
		mParent->SetWorldRotationMatrix(viewMatrix);

		// 播放序列帧
		if(mTextureNameList.size() > 1 && mDuration > 0.0f)
		{
			// 第一次播放序列帧
			if(mCurTexture == NULL)
			{
				mCurTextureIndex = 0;
				mCurTexture = mTextureManager->getTexture(mTextureNameList[mCurTextureIndex]);
				mCurPlayTime = 0.0f;
			}
			else
			{
				// 累加当前播放时间
				mCurPlayTime += fElaspedTime;
				// 播放时间已经超过了总时间,从头开始播放
				if(mCurPlayTime >= mDuration)
				{
					// 需要自动销毁
					if(mAutoDestroy)
					{
						mBillboardManager->DestroyBillboard(getName());
						return;
					}
					else if(mHideAndStopPlayEnd)
					{
						stop();
					}
					else
					{
						mCurTextureIndex = 0;
						mCurTexture = mTextureManager->getTexture(mTextureNameList[mCurTextureIndex]);
						mCurPlayTime = 0.0f;
					}
				}
				// 还要继续播放, 计算当前显示的纹理
				else
				{
					mCurTextureIndex = (int)(mCurPlayTime / (mDuration / mTextureNameList.size()));
					mCurTexture = mTextureManager->getTexture(mTextureNameList[mCurTextureIndex]);
				}
			}
		}
		// 显示单张图片
		else if(mTextureNameList.size() == 1)
		{
			mCurTextureIndex = 0;
			mCurTexture = mTextureManager->getTexture(mTextureNameList[mCurTextureIndex]);
		}
	}
}

void txBillboard::render()
{
	if(mParent == NULL || mCamera == NULL)
	{
		return;
	}
	// 如果设置为隐藏,则不渲染
	if(!mVisible)
	{
		return;
	}
	if(mCurTexture == NULL)
	{
		return;
	}
	MATRIX4 transitionMatrix = txMath::translate(txMath::MAT4_IDENTITY, mParent->GetWorldPosition());
	MATRIX4 MMatrix = transitionMatrix * MATRIX4(mParent->GetWorldRotationMatrix());
	mTextureRenderer->render(mCurTexture, NULL, mCamera->getVPMatrix() * MMatrix, (const GLfloat*)mVertices, (const GLfloat*)mTexCoord,
		(const GLushort*)mIndices, 6, mAlpha, mColour, mColourBlendMode, txMath::VEC3_ZERO, 1.0f, false);
}