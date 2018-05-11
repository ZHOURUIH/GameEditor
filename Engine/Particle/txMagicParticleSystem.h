/*
 * txMagicParticleSystem.h
 *
 *  Created on: 2015-6-25
 *      Author: zhourui
 */

#ifndef _TX_MAGIC_PARTICLE_SYSTEM_H_
#define _TX_MAGIC_PARTICLE_SYSTEM_H_

#include "txMP_wrap.h"

class txMP_Device;

// txMagicParticleSystem是对MP_Manager的再一次封装,外部只能通过这个类来对粒子特效进行操作,不能调用MP_Manager中的函数进行操作
// 不过可以获得MP_Emitter来进行操作
class txMagicParticleSystem
{
public:
	txMagicParticleSystem();
	virtual ~txMagicParticleSystem();
	// 初始化粒子系统
	virtual void init();
	// 加载固定路径中的所有的粒子文件
	void LoadAllEmitters();
	// 加载发射器,加载后就会立即更新发射器发射粒子,parent为发射器节点的父节点
	void LoadEmitter(const char* ptcFile, txNode* parent, const bool& changeParticleByEmitter = true, const bool& interpolation = false);
	// 根据一个已经加载的粒子文件,创建相同的发射器,返回值是拷贝出的文件名(实际并没有另外创建文件),用于根据文件名和发射器名(发射器名与源发射器名的相同)获取发射器
	std::string LoadEmitterByLoadedFile(const char* ptcFile, txNode* parent, const bool& changeParticleByEmitter = true, const bool& interpolation = false);
	// 卸载发射器,会将发射器从内存中销毁掉
	void UnLoadEmitter(const HM_EMITTER& hEmitter);
	// 得到一个粒子文件中的所有发射器句柄
	txMap<std::string, HM_EMITTER>& GetAllEmittersFromFile(const std::string& ptcFile);
	// 设置一个粒子文件中所有发射器的父节点
	void SetEmitterParentNode(const std::string& ptcFile, txNode* parent);
	// 设置发射器的父节点
	void SetEmitterParentNode(const HM_EMITTER& hEmitter, txNode* parent);
	// 停止全部的发射器
	void StopAllEmitter();
	// 设置一个粒子文件中所有发射器的状态
	bool SetEmitterState(const std::string& ptcFile, const EMITTER_STATE& state);
	// 设置发射器状态
	bool SetEmitterState(const HM_EMITTER& hEmitter, const EMITTER_STATE& state);
	// 设置一个粒子文件中所有发射器循环状态
	bool SetEmitterLoopMode(const std::string& ptcFile, const EMITTER_UPDATE_MODE& loop);
	// 设置发射器循环状态
	bool SetEmitterLoopMode(const HM_EMITTER& hEmitter, const EMITTER_UPDATE_MODE& loop);
	// 播放一个粒子文件中所有的发射器
	void PlayEmitter(const std::string& ptcFile);
	// 播放发射器
	void PlayEmitter(const HM_EMITTER& hEmitter);
	// 停止一个粒子文件中所有的发射器
	void StopEmitter(const std::string& ptcFile);
	// 停止发射器
	void StopEmitter(const HM_EMITTER& hEmitter);
	// 得到发射器
	MP_Emitter* GetEmitter(const HM_EMITTER& hEmitter);
	// 根据粒子文件名和发射器名得到发射器的指针
	MP_Emitter* GetEmitterPointerByName(const std::string& ptcFile, const std::string& emitterName);
	// 根据粒子文件名和发射器名得到发射器的句柄
	HM_EMITTER GetEmitterHandleByName(const std::string& ptcFile, const std::string& emitterName);
	// 返回指定的粒子文件是否已经加载
	bool IsParticleFileLoaded(const std::string& ptcFile) { return mParticleFileList.find(ptcFile) != mParticleFileList.end(); }
	// 渲染粒子
	void RenderParticles(txCamera* camera);

protected:
	txMP_Device* mDevice;
	MP_Manager* mParticleManager;
	// 虽然在MP_Manager中也保存了发射器的句柄,但是这里保存的用于更方便地获取
	txMap<std::string, PtcStruct> mParticleFileList;
	static txMap<std::string, HM_EMITTER> EMPTY_EMITTER_LIST;
};

#endif