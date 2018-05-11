#include "txOpenGLBuffer.h"

OpenGLBuffer::OpenGLBuffer()
{
	k_particles = 0;
	max_particles = 0;

	m_vertex = NULL;
	m_index = NULL;

#ifdef OPENGL_SHADER
	glGenBuffers(2, buffers);
#endif
}

OpenGLBuffer::~OpenGLBuffer()
{
	if (m_vertex)
	{
		free(m_vertex);
		m_vertex = NULL;
	}

	if (m_index)
	{
		free(m_index);
		m_index = NULL;
	}

	k_particles = 0;
	max_particles = 0;

#ifdef OPENGL_SHADER
	glDeleteBuffers(2, buffers);
#endif
}

void OpenGLBuffer::Create(const int& count)
{
	if (count > max_particles)
	{
		m_vertex = (OPENGL_VERTEX*)realloc(m_vertex, sizeof(OPENGL_VERTEX)*count * 4);
		m_index = (GLushort*)realloc(m_index, sizeof(GLushort)*count * 6);

		for (int i = max_particles; i < count; ++i)
		{
			int vertex = i * 4;
			GLushort* ind = &(m_index[i * 6]);
			*ind = vertex;
			++ind;
			*ind = vertex + 1;
			++ind;
			*ind = vertex + 2;
			++ind;
			*ind = vertex + 3;
			++ind;
			*ind = vertex;
			++ind;
			*ind = vertex + 2;
			++ind;
		}

		max_particles = count;

#ifdef OPENGL_SHADER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)* count * 6, m_index, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
	}
	k_particles = 0;
};