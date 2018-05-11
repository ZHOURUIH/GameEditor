/*
 * txMP_Atlas.cpp
 *
 *  Created on: 2015-6-25
 *      Author: zhourui
 */
#include "txMP_wrap.h"
#include "txMP_Atlas.h"
#include "txTexture.h"
#include "txOpenGLBuffer.h"
#include "txMP_Device.h"
#include "txCamera.h"
#include "txTextureManager.h"

#ifdef OPENIL
#include "IL/ilu.h"
#endif

#ifdef COCOS_2DX
	using namespace cocos2d;
#else
	#ifdef _WINDOWS
	#include <windows.h>
	#pragma comment(lib, "opengl32.lib")
	#endif
#endif


#ifdef OPENIL
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#else
#endif

#pragma warning ( disable : 4996)

#define offset_of(st, m) ((size_t)(&((st *)0)->m))

// eng: Destroy atlas texture
void txMP_Atlas::Destroy()
{
	if (texture)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
	if (mTexture)
	{
		mTextureManager->destroyTexture(mTexture->getName());
		mTexture = NULL;
	}
}

void txMP_Atlas::CreateSubTexture(unsigned char* pixels, const int& sourceWidth, const int& sourceHeight, const int& x, const int& y, const int& width, const int& height)
{
	if (sourceWidth != width || sourceHeight != height)
	{
		int size = width * height * 4;
		unsigned int* to = (unsigned int*)malloc(size);
		unsigned int* from = (unsigned int*)pixels;

		float scale_x = ((float)width) / sourceWidth;
		float scale_y = ((float)height) / sourceHeight;

		int pitch_to = width;
		int pitch_from = sourceWidth;

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				int i2 = (int)(((float)i) / scale_x);
				int j2 = (int)(((float)j) / scale_y);

				unsigned int color = from[j2 * pitch_from + i2];
				to[j * pitch_to + i] = color;
			}
		}

		pixels = (unsigned char*)to;
	}

	if(MP_Manager::mTextureType == TT_SWI)
	{
		BindTexture(mTexture->getTextureID());
	}
	else if(MP_Manager::mTextureType == TT_PNG)
	{
		BindTexture(texture);
	}

	GLint unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
	free(pixels);
}

// eng: Cleaning up of rectangle
void txMP_Atlas::CleanRectangle(const MAGIC_CHANGE_ATLAS* c)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	if(MP_Manager::mTextureType == TT_SWI)
	{
		BindTexture(mTexture->getTextureID());
	}
	else if(MP_Manager::mTextureType == TT_PNG)
	{
		BindTexture(texture);
	}

	int size = c->width * c->height * 4;
	unsigned char* null_data = TRACE_NEW_ARRAY(unsigned char, size, null_data);
	for (int i = 0; i < size; ++i)
	{
		null_data[i] = 0;
	}

	GLint unpack_alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, c->x, c->y, c->width, c->height, GL_RGBA, GL_UNSIGNED_BYTE, null_data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

	TRACE_DELETE_ARRAY(null_data);

	BindTexture(prev_texture);
}

// eng: Particle drawing
void txMP_Atlas::Draw(MAGIC_PARTICLE_VERTEXES* vertexes)
{
    OpenGLBuffer& buffer = device->GetBuffer();

	OPENGL_VERTEX* vertex = buffer.NextParticles();
	GLuint color = vertexes->color;
	// eng: exchange Red and Blue
	GLubyte* to = (GLubyte*)&color;
	GLubyte* from = (GLubyte*)&(vertexes->color);
	to[0] = from[2];
	to[2] = from[0];

	#ifdef MAGIC_3D
	vertex->x = vertexes->vertex1.x;
	vertex->y = vertexes->vertex1.y;
	vertex->u = vertexes->u1;
	vertex->v = vertexes->v1;
	vertex->color = color;
	vertex->z = -vertexes->vertex1.z;
	++vertex;

	vertex->x = vertexes->vertex2.x;
	vertex->y = vertexes->vertex2.y;
	vertex->u = vertexes->u2;
	vertex->v = vertexes->v2;
	vertex->color = color;
	vertex->z = -vertexes->vertex2.z;
	++vertex;

	vertex->x = vertexes->vertex3.x;
	vertex->y = vertexes->vertex3.y;
	vertex->u = vertexes->u3;
	vertex->v = vertexes->v3;
	vertex->color = color;
	vertex->z = -vertexes->vertex3.z;
	++vertex;

	vertex->x = vertexes->vertex4.x;
	vertex->y = vertexes->vertex4.y;
	vertex->u = vertexes->u4;
	vertex->v = vertexes->v4;
	vertex->color = color;
	vertex->z = -vertexes->vertex4.z;
	++vertex;

	#else
//	vertex->x = vertexes->vertex1.x;
//	vertex->y = vertexes->vertex1.y;
//	vertex->u = vertexes->u1;
//	vertex->v = vertexes->v1;
//	vertex->color = color;
//	vertex++;
//
//	vertex->x = vertexes->vertex2.x;
//	vertex->y = vertexes->vertex2.y;
//	vertex->u = vertexes->u2;
//	vertex->v = vertexes->v2;
//	vertex->color = color;
//	vertex++;
//
//	vertex->x = vertexes->vertex3.x;
//	vertex->y = vertexes->vertex3.y;
//	vertex->u = vertexes->u3;
//	vertex->v = vertexes->v3;
//	vertex->color = color;
//	vertex++;
//
//	vertex->x = vertexes->vertex4.x;
//	vertex->y = vertexes->vertex4.y;
//	vertex->u = vertexes->u4;
//	vertex->v = vertexes->v4;
//	vertex->color = color;
//	vertex++;
	#endif
};


// eng: Setting of intense
void txMP_Atlas::SetIntense(const bool& intense)
{
	if (intense)
	{
		// eng: turn on intense
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else
	{
		// eng: turn off intense
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

// eng: Beginning of emitter drawing
void txMP_Atlas::BeginDrawEmitter(MP_Emitter* emitter)
{
	if(MP_Manager::mTextureType == TT_SWI)
	{
		BindTexture(mTexture->getTextureID());
	}
	else if(MP_Manager::mTextureType == TT_PNG)
	{
		BindTexture(texture);
	}
}

// eng: Beginning of particles drawing
void txMP_Atlas::BeginDrawParticles(const int& particles_count)
{
    OpenGLBuffer& buffer = device->GetBuffer();
	buffer.Create(particles_count);
}

// eng: End of particles drawing
void txMP_Atlas::EndDrawParticles()
{
	MP_Atlas::EndDrawParticles();
    OpenGLBuffer& buffer = device->GetBuffer();
	int k_particles = buffer.GetCount();
	if (k_particles > 0)
	{
#ifdef OPENGL_SHADER

		buffer.CopyBuffersToGPU();
		GLsizei step = sizeof(OPENGL_VERTEX);

		int offset_pos = offset_of(OPENGL_VERTEX, x);
		int offset_color = offset_of(OPENGL_VERTEX, color);
		int offset_uv = offset_of(OPENGL_VERTEX, u);

//#ifdef MAGIC_3D
//		glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, step, (GLvoid*)offset_pos);
//#else
//		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, step, (GLvoid*)offset_pos);
//#endif
//		glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, step, (GLvoid*)offset_color); //enable the normalized flag
//		glVertexAttribPointer(ATTRIB_TEXTUREPOSITION, 2, GL_FLOAT, 0, step, (GLvoid*)offset_uv);
#ifdef MAGIC_3D
		glVertexAttribPointer(device->GetPositionHandle(), 3, GL_FLOAT, 0, step, (GLvoid*)offset_pos);
#else
		glVertexAttribPointer(device->GetPositionHandle(), 2, GL_FLOAT, 0, step, (GLvoid*)offset_pos);
#endif
		glVertexAttribPointer(device->GetColourHandle(), 4, GL_UNSIGNED_BYTE, 1, step, (GLvoid*)offset_color); //enable the normalized flag
		glVertexAttribPointer(device->GetTextureCoordHandle(), 2, GL_FLOAT, 0, step, (GLvoid*)offset_uv);

		glDrawElements(GL_TRIANGLES, k_particles * 6, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

#else

		OPENGL_VERTEX* ar = buffer.GetArray();

		GLsizei step = sizeof(OPENGL_VERTEX);

		float* first_x = &(ar->x);

#ifdef MAGIC_3D
		glVertexPointer(3, GL_FLOAT, step, first_x);
#else
		glVertexPointer(2, GL_FLOAT, step, first_x);
#endif

		float* first_u = &(ar->u);
		glTexCoordPointer(2, GL_FLOAT, step, first_u);

		GLuint* first_color = &(ar->color);
		glColorPointer(4, GL_UNSIGNED_BYTE, step, first_color);

		glDrawElements(GL_TRIANGLES, k_particles * 6, GL_UNSIGNED_SHORT, buffer.GetIndexArray());
#endif
	}
}
