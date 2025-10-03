#include "AllHeader.h"

void UndoManager::destroy()
{
	clearUndo();
	clearRedo();
}

void UndoManager::addUndo(Undo* undoCollection)
{
	// ���ڽ��г�������,������Ӧ������������
	if (mUndoing)
	{
		const bool beforeRedoEnable = canRedo();
		mRedoBuffer.push_back(undoCollection);
		redoChanged(beforeRedoEnable, canRedo());
	}
	// û�����ڳ���,����볷������
	else
	{
		const bool beforeUndoEnable = canUndo();
		// �������ĳ��������Ѿ��������������,��ɾ�����ȼ���ĳ�������
		if (mUndoBuffer.size() >= EditorDefine::MAX_UNDO_COUNT)
		{
			delete *mUndoBuffer.begin();
			mUndoBuffer.erase(mUndoBuffer.begin());
		}
		mUndoBuffer.push_back(undoCollection);
		undoChanged(beforeUndoEnable, canUndo());

		// �������������ʱ��ӳ�������,����Ҫ��������б�
		if (!mRedoing)
		{
			clearRedo();
		}
	}
}

void UndoManager::undo()
{
	mUndoing = true;
	if (canUndo())
	{
		// ִ���˳����������ɾ���ò���
		const bool beforeUndoEnable = canUndo();
		mUndoBuffer[mUndoBuffer.size() - 1]->undo();
		delete *(mUndoBuffer.begin() + mUndoBuffer.size() - 1);
		mUndoBuffer.erase(mUndoBuffer.begin() + mUndoBuffer.size() - 1);
		undoChanged(beforeUndoEnable, canUndo());
	}
	mUndoing = false;
}

void UndoManager::redo()
{
	mRedoing = true;
	if (canRedo())
	{
		// ִ���������������ɾ���ò���
		const bool beforeRedoEnable = canRedo();
		mRedoBuffer[mRedoBuffer.size() - 1]->undo();
		delete *(mRedoBuffer.begin() + mRedoBuffer.size() - 1);
		mRedoBuffer.erase(mRedoBuffer.begin() + mRedoBuffer.size() - 1);
		redoChanged(beforeRedoEnable, canRedo());
	}
	mRedoing = false;
}

void UndoManager::clearRedo()
{
	bool beforeRedoEnable = canRedo();
	for (Undo* item : mRedoBuffer)
	{
		delete item;
	}
	mRedoBuffer.clear();
	redoChanged(beforeRedoEnable, canRedo());
}

void UndoManager::clearUndo()
{
	const bool beforeUndoEnable = canUndo();
	for (Undo* item : mUndoBuffer)
	{
		delete item;
	}
	mUndoBuffer.clear();
	undoChanged(beforeUndoEnable, canUndo());
}

void UndoManager::checkUndoRedoEnable()
{
	undoChanged(false, canUndo(), true);
	redoChanged(false, canRedo(), true);
}

void UndoManager::undoChanged(bool beforeEnable, bool nowEnable, bool force)
{
	if (beforeEnable != nowEnable || force)
	{
		CALL(mUndoChangeCallback);
	}
}

void UndoManager::redoChanged(bool beforeEnable, bool nowEnable, bool force)
{
	if (beforeEnable != nowEnable || force)
	{
		CALL(mUndoChangeCallback);
	}
}