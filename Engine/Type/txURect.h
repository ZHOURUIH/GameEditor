#ifndef _TX_URECT_H_
#define _TX_URECT_H_

#include "txPoint.h"

class txURect
{
public:
	txURect(){}
	virtual ~txURect(){}
public:
	txPoint mMin;
	txPoint mMax;
};

#endif