#ifndef _TX_COMONENT_STATIC_MESH_H_
#define _TX_COMONENT_STATIC_MESH_H_

#include "txComponentMesh.h"

// 静态模型组件
class txComponentStaticMesh : public txComponentMesh
{
public:
	txComponentStaticMesh(const std::string& type, const std::string& name)
		:
		txComponentMesh(type, name)
	{}
	virtual ~txComponentStaticMesh(){ destroy(); }
	virtual void initProperty();
	virtual bool isType(const std::string& type)
	{
		return txComponentMesh::isType(type) || type == TOSTRING(txComponentStaticMesh);
	}
	void destroy(){}
protected:
};

#endif