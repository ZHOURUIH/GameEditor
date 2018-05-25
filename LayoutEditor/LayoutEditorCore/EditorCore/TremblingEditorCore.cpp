#include "txTremblingManager.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"
#include "txTrembling.h"
#include "Utility.h"

#include "TremblingEditorCore.h"
#include "EditorCoreRoot.h"

TremblingEditorCore::TremblingEditorCore()
:
mCurTrembling(NULL),
mCurTremblingNode(NULL),
mCurKeyFrame(NULL),
mTremblingModified(false)
{
	;
}

void TremblingEditorCore::createKeyFrameTrembling(const std::string& name)
{
	if (mTremblingModified && mCurTrembling != NULL)
	{
		askSaveTrembling("是否保存当前的关键帧震动?");
	}
	selectKeyFrameDimension(EMPTY_STRING);
	mCurTrembling = mTremblingManager->createTrembling(name);
	if (mCurTrembling == NULL)
	{
		return;
	}
	// 发送新建震动的事件
	mEditorCoreRoot->sendEvent(CE_NEW_TREMBLING, mCurTrembling->getName());
	// 设置为已修改状态
	setKeyFrameModified(true);
}

void TremblingEditorCore::loadKeyFrameTrembling(const std::string& tremblingName)
{
	if (mTremblingModified && mCurTrembling != NULL && mCurTrembling->getName() != tremblingName)
	{
		askSaveTrembling("是否保存当前的关键帧震动?");
	}
	mCurTrembling = mTremblingManager->getTrembling(tremblingName);
	selectKeyFrameDimension(EMPTY_STRING);
	// 发送加载震动的事件
	mEditorCoreRoot->sendEvent(CE_LOAD_TREMBLING, tremblingName);
	// 最后要发送通知事件
	setKeyFrameModified(false, true);
}

// 选择一个关键帧维度
void TremblingEditorCore::selectKeyFrameDimension(const std::string& dimensionName)
{
	if (mCurTrembling != NULL)
	{
		mCurTremblingNode = mCurTrembling->getNode(dimensionName);
	}
	selectKeyFrame(-1);
	mEditorCoreRoot->sendEvent(CE_SELECT_TREMBLING_NODE, dimensionName);
}

void TremblingEditorCore::addDimension(const std::string& dimensionName)
{
	if (mCurTrembling != NULL)
	{
		txTremblingNode* node = mCurTrembling->addNode(dimensionName, EMPTY_STRING, txMap<float, float>());
		if (node != NULL)
		{
			selectKeyFrameDimension(node->getName());
			// 发送添加节点的事件
			mEditorCoreRoot->sendEvent(CE_ADD_NODE, mCurTremblingNode->getName());
			setKeyFrameModified(true);
		}
	}
}

void TremblingEditorCore::deleteDimension(const std::string& dimensionName)
{
	if (mCurTrembling != NULL)
	{
		mCurTrembling->deleteNode(dimensionName);
		selectKeyFrameDimension(EMPTY_STRING);
		// 发送添加节点的事件
		mEditorCoreRoot->sendEvent(CE_DELETE_NODE, mCurTremblingNode->getName());
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::setKeyFrameModified(const bool& modified, const bool& force, const bool& sendEvt)
{
	if (mTremblingModified != modified || force)
	{
		mTremblingModified = modified;
		if (sendEvt)
		{
			// 发送修改状态改变的事件
			txVector<std::string> params;
			params.push_back(mCurTrembling ? mCurTrembling->getName() : EMPTY_STRING);
			params.push_back(txStringUtility::boolToString(mTremblingModified));
			mEditorCoreRoot->sendEvent(CE_TREMBLE_MODIFIED, params);
		}
	}
}

void TremblingEditorCore::saveKeyFrameTrebling()
{
	if (mCurTrembling == NULL || !mTremblingModified)
	{
		return;
	}
	std::string stream;
	mCurTrembling->saveTrembling(stream);
	std::string filePath = P_TREMBLING_PATH + mCurTrembling->getName() + TREMBLING_SUFFIX;
	txFileUtility::writeFile(filePath, stream.length(), stream.c_str());
	setKeyFrameModified(false, true, true);
}

void TremblingEditorCore::selectKeyFrame(const int& index)
{
	if (mCurTrembling == NULL || mCurTremblingNode == NULL || mCurKeyFrame == mCurTremblingNode->getKeyFrame(index))
	{
		return;
	}

	if (index >= 0 && index < mCurTremblingNode->getKeyFrameCount())
	{
		mCurKeyFrame = mCurTremblingNode->getKeyFrame(index);
	}
	else
	{
		mCurKeyFrame = NULL;
	}
	mEditorCoreRoot->sendEvent(CE_SELECT_KEY_FRAME, txStringUtility::intToString((int)mCurKeyFrame));
}

void TremblingEditorCore::setKeyFrameTime(const float& time)
{
	if (mCurKeyFrame != NULL)
	{
		mCurKeyFrame->setTime(time);
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::setKeyFrameValue(const float& value)
{
	if (mCurKeyFrame != NULL)
	{
		mCurKeyFrame->setOffset(value);
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::setTremblingInfo(const std::string& info)
{
	if (mCurTrembling != NULL)
	{
		mCurTrembling->setInfo(info);
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::setTremblingFormat(const std::string& format)
{
	if (mCurTrembling != NULL)
	{
		mCurTrembling->setFormat(format);
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::setTremblingNodeInfo(const std::string& info)
{
	if (mCurTremblingNode != NULL)
	{
		mCurTremblingNode->setInfo(info);
		setKeyFrameModified(true);
	}
}

void TremblingEditorCore::addKeyFrame(const int& index, const float& time, const float& offset)
{
	if (mCurTremblingNode != NULL)
	{
		if (mCurTremblingNode->insertKeyFrame(index, time, offset) != NULL)
		{
			mEditorCoreRoot->sendEvent(CE_ADD_KEY_FRAME, txStringUtility::intToString(index));
			setKeyFrameModified(true);
		}
	}
}

void TremblingEditorCore::deleteKeyFrame(const int& index)
{
	if (mCurTremblingNode != NULL)
	{
		// 因为index是被销毁的关键帧中的成员变量的引用, 所以需要用临时的变量
		int tempIndex = index;
		if (mCurKeyFrame == mCurTremblingNode->getKeyFrame(tempIndex))
		{
			mCurKeyFrame = NULL;
		}
		if (mCurTremblingNode->deleteKeyFrame(tempIndex))
		{
			mEditorCoreRoot->sendEvent(CE_DELETE_KEY_FRAME, txStringUtility::intToString(tempIndex));
			setKeyFrameModified(true);
		}
		selectKeyFrame(std::min(mCurTremblingNode->getKeyFrameCount() - 1, tempIndex));
	}
}

bool TremblingEditorCore::askSaveTrembling(const std::string& info)
{
	MODAL_DIALOG_RESULT ret = mEditorCoreRoot->showModalYesNoCancel(info);
	// 点击是,则保存文件
	if (ret == MDR_YES)
	{
		saveKeyFrameTrebling();
	}
	// 点击否,则标记当前布局为未修改状态
	else if (ret == MDR_NO)
	{
		setKeyFrameModified(false, false, false);
	}
	// 点击取消则不打开文件
	else
	{
		return false;
	}
	return true;
}