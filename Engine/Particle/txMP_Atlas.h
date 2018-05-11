#ifndef TXMP_ATLAS_H
#define TXMP_ATLAS_H

#include "txMP_wrap.h"

class txTexture;
class txMP_Atlas : public MP_Atlas
{
protected:
	txTexture* mTexture;
	GLuint texture;

public:
	txMP_Atlas(const int& width, const int& height, const char* file);
	virtual ~txMP_Atlas(){ Destroy(); }

	// eng: Destroy atlas texture
	virtual void Destroy();

	// eng: Loading of frame texture
	virtual void LoadTexture(const MAGIC_CHANGE_ATLAS* c);

	// eng: Cleaning up of rectangle
	virtual void CleanRectangle(const MAGIC_CHANGE_ATLAS* c);

	// 从一个完整的图片中的指定区域创建一张图片到opengl
	void CreateSubTexture(unsigned char* pixels, const int& sourceWidth, const int& sourceHeight, const int& x, const int& y, const int& width, const int& height);

	// eng: Particle drawing
	virtual void Draw(MAGIC_PARTICLE_VERTEXES* vertex_rectangle);

	// eng: Beginning of emitter drawing
	virtual void BeginDrawEmitter(MP_Emitter* emitter);

	// eng: Beginning of particles drawing
	virtual void BeginDrawParticles(const int& particles_count);

	// eng: End of particles drawing
	virtual void EndDrawParticles();

	// eng: Setting of intense
	virtual void SetIntense(const bool& intense);
};

#endif