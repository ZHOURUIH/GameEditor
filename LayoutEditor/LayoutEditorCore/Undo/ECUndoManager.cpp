#include "Utility.h"

#include "ECUndoManager.h"
#include "ECUndo.h"
#include "CommonDefine.h"
#include "EditorCoreRoot.h"

void ECUndoManager::destroy()
{
	clearUndo();
	clearRedo();
}

void ECUndoManager::addUndo(ECUndo* undoCollection)
{
	// 正在进行撤销操作,则加入的应该是重做操作
	if (mUndoing)
	{
		bool beforeRedoEnable = canRedo();
		mRedoBuffer.push_back(undoCollection);
		redoChanged(beforeRedoEnable, canRedo());
	}
	// 没有正在撤销,则加入撤销操作
	else
	{
		bool beforeUndoEnable = canUndo();
		// 如果加入的撤销操作已经大于最大数量了,则删除最先加入的撤销操作
		if (mUndoBuffer.size() >= MAX_UNDO_COUNT)
		{
			TRACE_DELETE(*mUndoBuffer.begin());
			mUndoBuffer.erase(mUndoBuffer.begin());
		}
		mUndoBuffer.push_back(undoCollection);
		undoChanged(beforeUndoEnable, canUndo());

		// 如果不是在重做时添加撤销操作,则需要清空重做列表
		if (!mRedoing)
		{
			clearRedo();
		}
	}
}

void ECUndoManager::undo()
{
	mUndoing = true;
	if (canUndo())
	{
		// 执行了撤销操作后就删除该操作
		bool beforeUndoEnable = canUndo();
		mUndoBuffer[mUndoBuffer.size() - 1]->undo();
		TRACE_DELETE(*(mUndoBuffer.begin() + mUndoBuffer.size() - 1));
		mUndoBuffer.erase(mUndoBuffer.begin() + mUndoBuffer.size() - 1);
		undoChanged(beforeUndoEnable, canUndo());
	}
	mUndoing = false;
}

void ECUndoManager::redo()
{
	mRedoing = true;
	if (canRedo())
	{
		// 执行了重做操作后就删除该操作
		bool beforeRedoEnable = canRedo();
		mRedoBuffer[mRedoBuffer.size() - 1]->undo();
		TRACE_DELETE(*(mRedoBuffer.begin() + mRedoBuffer.size() - 1));
		mRedoBuffer.erase(mRedoBuffer.begin() + mRedoBuffer.size() - 1);
		redoChanged(beforeRedoEnable, canRedo());
	}
	mRedoing = false;
}

void ECUndoManager::clearRedo()
{
	bool beforeRedoEnable = canRedo();
	int redoCount = mRedoBuffer.size();
	FOR_STL(mRedoBuffer, int i = 0; i < redoCount; ++i)
	{
		TRACE_DELETE(mRedoBuffer[i]);
	}
	END_FOR_STL(mRedoBuffer);
	mRedoBuffer.clear();
	redoChanged(beforeRedoEnable, canRedo());
}

void ECUndoManager::clearUndo()
{
	bool beforeUndoEnable = canUndo();
	int undoCount = mUndoBuffer.size();
	FOR_STL(mUndoBuffer, int i = 0; i < undoCount; ++i)
	{
		TRACE_DELETE(mUndoBuffer[i]);
	}
	END_FOR_STL(mUndoBuffer);
	mUndoBuffer.clear();
	undoChanged(beforeUndoEnable, canUndo());
}

void ECUndoManager::checkUndoRedoEnable()
{
	undoChanged(false, canUndo(), true);
	redoChanged(false, canRedo(), true);
}

void ECUndoManager::undoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force)
{
	if (beforeEnable != nowEnable || force)
	{
		txVector<std::string> params;
		params.push_back(txStringUtility::boolToString(nowEnable));
		mEditorCoreRoot->sendEvent(CE_UNDO_ENABLE_CHANGE, params);
	}
}

void ECUndoManager::redoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force)
{
	if (beforeEnable != nowEnable || force)
	{
		txVector<std::string> params;
		params.push_back(txStringUtility::boolToString(nowEnable));
		mEditorCoreRoot->sendEvent(CE_REDO_ENABLE_CHANGE, params);
	}
}