#ifndef _TX_CONFIG_H_
#define _TX_CONFIG_H_

#include <string>

#define DATA_SLOTS 1024 //最大允许同时载入的资源数
#define DATA_NAME_LENGTH 128 //资源名称最大字节数

// 是否使用安全版本的API,暂时只有windows平台才使用安全版本的函数
#ifdef _WINDOWS
#define _USE_SAFE_API
#endif

// 平台标识宏
#define PLATFORM_WINDOWS 0
#define PLATFORM_ANDROID 1

// 正在运行的平台标识
#ifdef _WINDOWS
#define RUN_PLATFORM PLATFORM_WINDOWS
#else
#define  RUN_PLATFORM PLATFORM_ANDROID
#endif

// 只有在安卓平台下才可能从AssetManager中加载资源
#if RUN_PLATFORM == PLATFORM_ANDROID
// 从AssetManager加载资源
#define LOAD_FROM_ASSETMANAGER
#endif

// 可以将t的名字转化为字符串
#define TOSTRING(t) #t

#endif
