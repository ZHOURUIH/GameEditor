#ifndef _FRAME_DEFINE_H_
#define _FRAME_DEFINE_H_

#include "FrameMacro.h"

// 由于需要简化代码,并且尽量不会引入第三方库,所以全局开放std命名空间
using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// 基础数据类型简化定义
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

// 自定义的基础头文件,因为这些头文件中可能会用到上面的宏,所以放在下面
#include "myVector.h"
#include "myMap.h"
#include "mySet.h"
#include "myStack.h"
#include "myQueue.h"
#include "mySafeVector.h"
#include "mySafeMap.h"
#include "mySafeSet.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------
class FrameDefine
{
public:
};

#endif