#ifndef _GYM_INFO_H_
#define _GYM_INFO_H_

#include "EditorCoreBase.h"
#include "txUtility.h"
#include "txCommandReceiver.h"

typedef void onRegisteCallback(int ret, void* userData);

class GymInfo : public EditorCoreBase, public txCommandReceiver
{
public:
	GymInfo();
	void init();
	// 0表示成功,1表示无法连接服务器,2表示出现错误
	void queryRegiste();
	void registeDevice();
public:
	std::string mGymName;		// 健身房名字
	std::string mDeviceCode;	// 设备码
	std::string mRegisteCode;	// 注册码
	std::string mRegisteDate;	// 注册日期
	bool mRegisted;				// 当前是否已注册
};

#endif