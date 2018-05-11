#include <stdlib.h>
#include <stdio.h>

#include "txMP_wrap.h"
#include "txMP_Atlas.h"
#include "txMP_Device.h"
#include "txTexture.h"
#include "txTextureManager.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"

#include "pngimage/stb_image.c"
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

void MattrixToQuaternion(float m[4][4], float* quat)
{
	float tr, s, q[4];
	int i, j, k;

	int nxt[3] = { 1, 2, 0 };
	//计算矩阵轨迹
	tr = m[0][0] + m[1][1] + m[2][2];
	//检查矩阵轨迹是正还是负
	if (tr > 0.0)
	{
		s = std::sqrt(tr + 1.0f);
		quat[3] = s / 2.0f;
		s = 0.5f / s;
		quat[0] = (m[1][2] - m[2][1]) * s;
		quat[1] = (m[2][0] - m[0][2]) * s;
		quat[2] = (m[0][1] - m[1][0]) * s;
	}
	else
	{
		//轨迹是负
		i = 0;
		if (m[1][1] > m[0][0])
		{
			i = 1;
		}
		if (m[2][2] > m[i][i])
		{
			i = 2;
		}
		j = nxt[i];
		k = nxt[j];
		s = std::sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if (s != 0.0f)
		{
			s = 0.5f / s;
		}
		q[3] = (m[j][k] - m[k][j]) * s;
		q[j] = (m[i][j] - m[j][i]) * s;
		q[k] = (m[i][k] - m[k][i]) * s;
		quat[0] = q[0];
		quat[1] = q[1];
		quat[2] = q[2];
		quat[3] = q[3];
	}

	// 单位化四元数
	float sqrtLen = std::sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3]);
	quat[0] /= sqrtLen;
	quat[1] /= sqrtLen;
	quat[2] /= sqrtLen;
	quat[3] /= sqrtLen;
}

// en: Matrixes 3x3 multiplication
void MagicMatrixMul3(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b)
{
	c->_11 = a->_11*b->_11 + a->_12*b->_21 + a->_13*b->_31;
	c->_21 = a->_21*b->_11 + a->_22*b->_21 + a->_23*b->_31;
	c->_31 = a->_31*b->_11 + a->_32*b->_21 + a->_33*b->_31;

	c->_12 = a->_11*b->_12 + a->_12*b->_22 + a->_13*b->_32;
	c->_22 = a->_21*b->_12 + a->_22*b->_22 + a->_23*b->_32;
	c->_32 = a->_31*b->_12 + a->_32*b->_22 + a->_33*b->_32;

	c->_13 = a->_11*b->_13 + a->_12*b->_23 + a->_13*b->_33;
	c->_23 = a->_21*b->_13 + a->_22*b->_23 + a->_23*b->_33;
	c->_33 = a->_31*b->_13 + a->_32*b->_23 + a->_33*b->_33;
}

void MagicMatrixMul(MAGIC_MATRIX* c, const MAGIC_MATRIX* a, const MAGIC_MATRIX* b)
{
	c->_11 = a->_11*b->_11 + a->_12*b->_21 + a->_13*b->_31 + a->_14*b->_41;
	c->_21 = a->_21*b->_11 + a->_22*b->_21 + a->_23*b->_31 + a->_24*b->_41;
	c->_31 = a->_31*b->_11 + a->_32*b->_21 + a->_33*b->_31 + a->_34*b->_41;
	c->_41 = a->_41*b->_11 + a->_42*b->_21 + a->_43*b->_31 + a->_44*b->_41;

	c->_12 = a->_11*b->_12 + a->_12*b->_22 + a->_13*b->_32 + a->_14*b->_42;
	c->_22 = a->_21*b->_12 + a->_22*b->_22 + a->_23*b->_32 + a->_24*b->_42;
	c->_32 = a->_31*b->_12 + a->_32*b->_22 + a->_33*b->_32 + a->_34*b->_42;
	c->_42 = a->_41*b->_12 + a->_42*b->_22 + a->_43*b->_32 + a->_44*b->_42;

	c->_13 = a->_11*b->_13 + a->_12*b->_23 + a->_13*b->_33 + a->_14*b->_43;
	c->_23 = a->_21*b->_13 + a->_22*b->_23 + a->_23*b->_33 + a->_24*b->_43;
	c->_33 = a->_31*b->_13 + a->_32*b->_23 + a->_33*b->_33 + a->_34*b->_43;
	c->_43 = a->_41*b->_13 + a->_42*b->_23 + a->_43*b->_33 + a->_44*b->_43;

	c->_14 = a->_11*b->_14 + a->_12*b->_24 + a->_13*b->_34 + a->_14*b->_44;
	c->_24 = a->_21*b->_14 + a->_22*b->_24 + a->_23*b->_34 + a->_24*b->_44;
	c->_34 = a->_31*b->_14 + a->_32*b->_24 + a->_33*b->_34 + a->_34*b->_44;
	c->_44 = a->_41*b->_14 + a->_42*b->_24 + a->_43*b->_34 + a->_44*b->_44;
}

void MagicMatrixTransposing(MAGIC_MATRIX* m)
{
	MAGIC_MATRIX v = *m;

	m->_12 = v._21;
	m->_13 = v._31;
	m->_14 = v._41;

	m->_21 = v._12;
	m->_23 = v._32;
	m->_24 = v._42;

	m->_31 = v._13;
	m->_32 = v._23;
	m->_34 = v._43;

	m->_41 = v._14;
	m->_42 = v._24;
	m->_43 = v._34;
}


#ifdef OPENGL_SHADER
void GetVertexBufferAttributes(GLuint index, BUFFER_ATTRIBUTES* bc)
{
		glGetVertexAttribPointerv(index, GL_VERTEX_ATTRIB_ARRAY_POINTER, &(bc->pointer));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &(bc->enabled));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_SIZE, &(bc->size));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_TYPE, &(bc->type));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &(bc->normalized));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &(bc->stride));
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &(bc->buffer_binding));
}
#endif

void BindTexture(const GLuint& textureId)
{
	#ifdef COCOS_2DX
	ccGLBindTexture2D(textureId);
	#else
	glBindTexture(GL_TEXTURE_2D, textureId);
	#endif
}


// eng: Class storing the textural atlas
txMP_Atlas::txMP_Atlas(const int& width, const int& height, const char* file)
:
MP_Atlas(width, height, file),
texture(0),
mTexture(NULL)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	if(MP_Manager::mTextureType == TT_SWI)
	{
		// 如果图片文件名为空,则不创建图片
		if (file_name != EMPTY_STRING)
		{
			std::string texturePath = MP->GetPathToTexture() + file_name;
			// 该图片资源由自己管理释放,不需要放到场景管理器中
			mTexture = mTextureManager->createTexture(texturePath, true, true);
			mTexture->initTexture();
		}
	}
	else if(MP_Manager::mTextureType == TT_PNG)
	{
		glGenTextures(1, &texture);
		BindTexture(texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (file_name.empty())
		{
			int size=width * height * 4;
			unsigned char* null_data = TRACE_NEW_ARRAY(unsigned char, size, null_data);

			for (int i = 0; i < size; ++i)
				null_data[i] = 0;

			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, null_data);
			TRACE_DELETE_ARRAY(null_data);
		}
		else
		{
			int wi, he;
			unsigned char* pixels = device->LoadTextureFromFile(file_name.c_str(), &wi, &he);
			if (pixels)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				stbi_image_free(pixels);
			}
		}
	}

	BindTexture(prev_texture);
}

// eng: Loading of frame texture
void txMP_Atlas::LoadTexture(const MAGIC_CHANGE_ATLAS* c)
{
	int prev_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_texture);

	if(MP_Manager::mTextureType == TT_SWI)
	{
		unsigned char* pixels=NULL;
		int wi_frame, he_frame;
		if(c->data == NULL)
		{
			// 如果图片还未创建,则先创建图片
			if(mTexture == NULL)
			{
				std::string texturePath = std::string(MP->GetPathToTexture()) + c->file;
				mTexture = mTextureManager->createTexture(texturePath, true, true);
				mTexture->initTexture();
			}
			pixels = (unsigned char*)(mTexture->GetTextureData());
			wi_frame = (int)mTexture->getTextureSize().x;
			he_frame = (int)mTexture->getTextureSize().y;
		}
		// 现在不知道c->data中的数据是固定的png格式的还是其他格式的,暂时认为是固定的png格式
		else
		{
			pixels = stbi_load_from_memory((const unsigned char*)c->data, c->length, &wi_frame, &he_frame, NULL, 4);
			// 如果图片还未创建,则根据像素数据创建图片
			if(mTexture->GetTextureData() == NULL)
			{
				mTexture->createCustomTexture((char*)pixels, wi_frame, he_frame, true);
			}
		}
		// 根据原始的完整的图片创建局部图片
		CreateSubTexture(pixels, wi_frame, he_frame, c->x, c->y, c->width, c->height);
	}
	else if(MP_Manager::mTextureType == TT_PNG)
	{
		unsigned char* pixels = NULL;
		int wi_frame, he_frame;
		if(c->data == NULL)
		{
			pixels = device->LoadTextureFromFile(c->file, &wi_frame, &he_frame);
		}
		// 运行到此处时c->data中的数据是png格式的
		else
		{
			pixels = stbi_load_from_memory((const unsigned char*)c->data, c->length, &wi_frame, &he_frame, NULL, 4);
		}
		CreateSubTexture(pixels, wi_frame, he_frame, c->x, c->y, c->width, c->height);
	}
	BindTexture(prev_texture);
}


#ifdef OPENIL
// eng: Loading texture from file
void txMP_Device::LoadTextureFromFile(const char* file_name)
{
	std::string texture_file=MP->GetPathToTexture();
	texture_file+=file_name;

	std::string mode="rb";
	const wchar_t* pmode=MP->utf8_to_wchar(mode.c_str());
	wchar_t wmode[3];
	wmode[0]=pmode[0];
	wmode[1]=pmode[1];
	wmode[2]=0;

	const wchar_t* wfile=MP->utf8_to_wchar(texture_file.c_str());

	FILE* file=_wfopen(wfile, wmode);
	fseek(file,0,SEEK_END);
	long len=ftell(file);
	fseek(file,0,SEEK_SET);

	char* data=TRACE_NEW_ARRAY(char, len, data);

	fread(data,1,len,file);
	fclose(file);

	ilLoadL(IL_TYPE_UNKNOWN,data,len);
	TRACE_DELETE_ARRAY(data);
}
#else

unsigned char* txMP_Device::LoadTextureFromFile(const char* file_name, int* width, int* height)
{
	if(MP_Manager::mTextureType != TT_PNG)
	{
		return NULL;
	}
	std::string texture_file = MP->GetPathToTexture();
	texture_file += file_name;

//	#if defined(__ANDROID__) && !defined(COCOS_2DX)
	int fileSize = 0;
	char* buffer = txFileUtility::openBinaryFile(texture_file, &fileSize);
	if(buffer == NULL)
	{
		ENGINE_ERROR("error : can not load file : %s", texture_file.c_str());
		return NULL;
	}
//	#else
//
//		#ifdef _WINDOWS
//
//			std::string mode="rb";
//			const wchar_t* pmode=MP->utf8_to_wchar(mode.c_str());
//			wchar_t wmode[3];
//			wmode[0]=pmode[0];
//			wmode[1]=pmode[1];
//			wmode[2]=0;
//
//			const wchar_t* wfile=MP->utf8_to_wchar(texture_file.c_str());
//
//			FILE* file=_wfopen(wfile, wmode);
//
//		#else
//
//			FILE* file=fopen(texture_file.c_str(), "rb");
//
//		#endif
//
//		fseek(file,0,SEEK_END);
//		long len=ftell(file);
//		fseek(file,0,SEEK_SET);
//
//		unsigned char* data=TRACE_NEW_ARRAY(unsigned char, len, data);
//
//		fread(data,1,len,file);
//		fclose(file);
//
//	#endif

	unsigned char* pixels=stbi_load_from_memory((unsigned char*)(buffer), fileSize, width, height, NULL, 4);
	if (!pixels)
	{
	    #ifdef OPENGL_WIN
		const char* error=stbi_failure_reason();
		MessageBox(NULL, error, "Magic Particles", MB_OK | MB_ICONERROR);
		#endif
	}
	TRACE_DELETE_ARRAY(buffer);
	return pixels;
}

#endif

