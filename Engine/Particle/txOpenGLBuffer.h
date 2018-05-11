#ifndef _TX_OPENGL_BUFFER_H_
#define _TX_OPENGL_BUFFER_H_

#include "txMP_wrap.h"

struct OPENGL_VERTEX
{
	GLfloat x, y;
#ifdef MAGIC_3D
	GLfloat z;
#endif
	GLfloat u, v;
	GLuint color;
};

// eng: Class of vertexes buffer
class OpenGLBuffer
{
protected:
#ifdef OPENGL_SHADER
	// VBO buffers
	GLuint buffers[2];
#endif

	int k_particles;
	int max_particles;

	OPENGL_VERTEX* m_vertex;

	GLushort* m_index;

public:
	OpenGLBuffer();

	virtual ~OpenGLBuffer();

	void Create(const int& count);

	const int& GetCount(){ return k_particles; }

	OPENGL_VERTEX* GetArray(){ return m_vertex; }

	GLushort* GetIndexArray(){ return m_index; }

	OPENGL_VERTEX* NextParticles()
	{
		OPENGL_VERTEX* vert = &(m_vertex[k_particles * 4]);
		++k_particles;
		return vert;
	}

#ifdef OPENGL_SHADER
	// put all data in GPU
	void CopyBuffersToGPU()
	{
		// target - 缓存对象要存储的数据类型，只有两个值： GL_ARRAY_BUFFER, 和 GL_ELEMENT_ARRAY_BUFFER。
		// 如果是顶点的相关属性，例如： 顶点坐标、纹理坐标、法线向量、颜色数组等，要使用 GL_ARRAY_BUFFER；
		// 索引数组，要使用 GL_ELEMENT_ARRAY_BUFFER，以便 glDrawElements() 使用。
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_VERTEX)* k_particles * 4, m_vertex, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	}
#endif
};

#endif