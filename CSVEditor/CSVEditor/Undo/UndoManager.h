#pragma once

#include "FrameDefine.h"

class Undo;
class UndoManager
{
public:
	UndoManager() = default;
	virtual ~UndoManager(){ destroy(); }
	void destroy();
	void addUndo(Undo* undoCollection);
	void undo();
	void redo();
	bool canUndo() const {return mUndoBuffer.size() > 0;}
	bool canRedo() const {return mRedoBuffer.size() > 0;}
	void setUndoChangeCallback(Action callback) { mUndoChangeCallback = callback; }
	void clearRedo();
	void clearUndo();
	void checkUndoRedoEnable();
protected:
	// beforeEnable�Ǹı�֮ǰ�Ŀɳ���״̬,nowEnable�Ǹı�֮��Ŀɳ���״̬,force������״̬��û�иı䶼ǿ�Ʒ����¼�
	void undoChanged(bool beforeEnable, bool nowEnable, bool force = false);
	void redoChanged(bool beforeEnable, bool nowEnable, bool force = false);
protected:
	Vector<Undo*> mUndoBuffer;
	Vector<Undo*> mRedoBuffer;
	bool mUndoing = false;	// �Ƿ����ڳ���,�������,����ӳ�������ʱʵ���������������
	bool mRedoing = false;	// �Ƿ���������,�����������,������ӳ�������ʱ��Ҫ�������б����
	Action mUndoChangeCallback;
};