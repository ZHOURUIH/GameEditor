#ifndef _TX_BILLBOARD_MANAGER_H_
#define _TX_BILLBOARD_MANAGER_H_

#include "txEngineDefine.h"

class txTexture;
class txCamera;
class txBillboard;
class txNode;
class txBillboardManager
{
public:
	txBillboardManager();
	virtual ~txBillboardManager();
	virtual void init(){}
	virtual void update(float elapsedTime);
	// name为创建的公告板的名称,需要保证该名称是唯一的
	// pos是公告板节点相对于父节点的位置
	// width, height是公告板的宽高
	// textureName是序列帧纹理名
	// frameCount是序列帧数,如果不是序列帧,则为1,如果不为1,则根据序列帧名称来加载图片,比如序列帧图片命名为a_0.swi, a_1.swi, a_2.swi,则textureName应为a_.swi,frameCount为3
	// pCamera是摄像机对象,用于获取视图和投影矩阵
	// parentNode是公告板节点的父节点
	// duration是一个序列帧循环的持续时间,如果不是序列帧则该参数无效
	txBillboard* CreateBillboard(const char* name, const VECTOR3& pos, const float& width, const float& height, const char* textureName, const int& frameCount, txCamera* pCamera, txNode* parentNode, const float& duration);
	txBillboard* GetBillboard(const char* name);
	// 停止全部公告板
	void StopAllBillboard();
	// 销毁公告板,并非是真正地销毁公告板,而是将公告板放到需要销毁的列表中,然后在场景的每一次循环最后才将这个列表中的公告板全部销毁
	void DestroyBillboard(const char* name);
	void DestroyAllBillboard();
	void RenderAllBillboard();
	// 销毁无用的公告板,需要在每一次循环的最后调用
	void ClearUselessBillboard();

protected:
	txMap<std::string, txBillboard*> mBillboardMap;
	txVector<txBillboard*> mDestroyBillboardList; // 需要被销毁的公告板列表,在每一次循环的最后将需要销毁的公告板销毁掉
};

#endif