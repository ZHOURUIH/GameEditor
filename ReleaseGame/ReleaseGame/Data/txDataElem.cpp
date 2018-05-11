#include <string.h>

#include "txDataElem.h"

txDataElem::txDataElem()
:mDataPtr(0),
 mSize(0)
{
	memset(k, 0, DATA_NAME_LENGTH);
}

txDataElem::~txDataElem()
{
	TRACE_DELETE_ARRAY(mDataPtr);
}

void txDataElem::setSize(int size)
{
	TRACE_DELETE_ARRAY(mDataPtr);

	if (size != 0)
	{
		mDataPtr = TRACE_NEW_ARRAY(char, size, mDataPtr);
		memset(mDataPtr, 0, size);
	}

	mSize = size;
}