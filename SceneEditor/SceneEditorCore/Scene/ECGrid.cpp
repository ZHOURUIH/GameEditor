#include "txNode.h"
#include "txMesh.h"
#include "txSubMesh.h"
#include "txNodeManager.h"
#include "txRendererManager.h"
#include "txCamera.h"
#include "txEngineRoot.h"
#include "txSceneManager.h"
#include "txEntityManager.h"

#include "ECGrid.h"
#include "ECEditor.h"
#include "txComponentHeader.h"

ECGrid::ECGrid(const std::string& name)
:
txMovableObject(name),
mVisible(false),
mAutoShow(true),
mGridColour(1.0f, 1.0f, 0.0f, 1.0f),
mVertices(NULL),
mIndices(NULL),
mIndexCount(0),
mAttachTarget(NULL)
{}

void ECGrid::initGrid(int width, int height)
{
	int mapWidth = width + 1;
	int mapHeight = height + 1;
	int size = mapWidth * mapHeight;
	mVertices = TRACE_NEW_ARRAY(GLfloat, size * 3, mVertices);
	for (int i = 0; i < size; ++i)
	{
		int x = i % mapWidth;
		int y = i / mapWidth;
		mVertices[i * 3 + 0] = (float)x - width / 2.0f;
		mVertices[i * 3 + 1] = 0.0f;
		mVertices[i * 3 + 2] = (float)y - height / 2.0f;
	}

	mIndexCount = mapWidth * 2 + mapHeight * 2;
	mIndices = TRACE_NEW_ARRAY(GLushort, mIndexCount, mIndices);
	memset(mIndices, 0, sizeof(GLushort) * mIndexCount);
	// 格子的横线
	for (int i = 0; i < mapHeight; ++i)
	{
		mIndices[i * 2 + 0] = mapWidth * i;
		mIndices[i * 2 + 1] = mapWidth * i + mapWidth - 1;
	}
	// 格子的竖线
	for (int i = 0; i < mapWidth; ++i)
	{
		mIndices[mapHeight * 2 + i * 2 + 0] = i;
		mIndices[mapHeight * 2 + i * 2 + 1] = mapWidth * (mapHeight - 1) + i;
	}
}

void ECGrid::update(float elapsedTime)
{
	txMovableObject::update(elapsedTime);
	if (mAttachTarget != NULL)
	{
		setPosition(mAttachTarget->getPosition(), false);
		setRotation(mAttachTarget->getRotation(), true);
	}
}

void ECGrid::attachTarget(ECEditor* editor)
{
	mAttachTarget = editor;
	// 自动隐藏显示时,没有挂接任何编辑体时,隐藏网格
	if (mAutoShow)
	{
		mVisible = mAttachTarget != NULL;
	}
}

void ECGrid::render(txCamera* camera)
{
	if (isVisible())
	{
		txRendererInstance::mColourRender->render(mGridColour, camera->getVPMatrix() * getNode()->GetFinalMatrix(), mVertices, mIndices, mIndexCount, 1.0f);
	}
}

void ECGrid::setAutoShow(bool autoShow)
{
	mAutoShow = autoShow;
	// 如果设置为自动显示,则判断当前是否应该显示
	if (mAutoShow)
	{
		mVisible = mAttachTarget != NULL;
	}
}