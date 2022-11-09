#ifndef _STACK_H_
#define _STACK_H_

#include "mySTL.h"
#include <stack>

using std::stack;

template<typename T>
class Stack : public mySTL
{
public:
	void push(const T& value) { mStack.push(value); }
	T pop()
	{
		T value = mStack.top();
		mStack.pop(); 
		return value;
	}
	T& top() { return mStack.top(); }
	int size() const { return (int)mStack.size(); }
public:
	stack<T> mStack;
};

#endif