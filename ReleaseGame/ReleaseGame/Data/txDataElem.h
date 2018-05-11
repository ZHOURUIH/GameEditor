#ifndef _TX_DATA_ELEM_H_
#define _TX_DATA_ELEM_H_

#include "txConfig.h"
#include "Utility.h"
#include "txMemeryTrace.h"

class txDataElem
{
protected:
	char k[DATA_NAME_LENGTH];
	char* mDataPtr;
	int mSize;

public:
	txDataElem();
	~txDataElem();

	void setMem(void* pMem)
	{
		TRACE_DELETE_ARRAY(mDataPtr);
		mDataPtr = (char*)pMem;
	}

	void setSize(int size);
	int getSize(){ return mSize; }

	void setValue(const char *val)
	{
#ifdef _USE_SAFE_API
		memcpy_s(mDataPtr, mSize, val, mSize);
#else
		memcpy(mDataPtr, val, mSize);
#endif
	}
	char* getValuePtr(){ return mDataPtr; }

	void setKey(const char *key)
	{
#ifdef _USE_SAFE_API
		strcpy_s(k, DATA_NAME_LENGTH, key);
#else
		strcpy(k, key);
#endif
	}
	char* getKeyPtr(){ return k; }
};

#endif
