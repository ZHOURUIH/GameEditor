#ifndef _TX_MATERIAL_H_
#define _TX_MATERIAL_H_

#include "txEngineDefine.h"

class txMaterial
{
public:
	txMaterial();
	virtual ~txMaterial(){}

	void operator = (const txMaterial& that);
	const int& getMaterialIndex() { return mIndex; }
	const bool& isWireFrame() { return mWireframe; }
	const std::string& getShadingMode() { return mShadingMode; }
	const std::string& getName() { return mMaterialName; }
	const float& getAnisotropy() { return mMapLightAnisotropy; }
	const std::string& getBlending() { return mBlending; }
	const VECTOR3& getDiffuseColour() { return mColorDiffuse; }
	const bool& getDepthTest() { return mDepthTest; }
	const VECTOR3& getEmissiveColour() { return mColorEmissive; }
	const bool& getDepthWrite() { return mDepthWrite; }
	const float& getOpacity() { return mOpacity; }
	const VECTOR3& getAmbientColour() { return mColorAmbient; }
	const std::string& getDiffuseTexture() { return mDiffuseTexture; }
	const std::string& getWrapU() { return mWrapU; }
	const std::string& getWrapV() { return mWrapV; }
	const int& getRepeatU() { return mRepeatU; }
	const int& getRepeatV() { return mRepeatV; }
	const bool& getTranspaent() { return mTransparent; }
	const float& getTextureOffsetU() { return mOffsetU; }
	const float& getTextureOffsetV() { return mOffsetV; }
	const int& getUVLayer() { return m_uvLayer; }

	void setMaterialIndex(const int& index) { mIndex = index; }
	void setWireframe(const bool& wireframe) { mWireframe = wireframe; }
	void setShadingMode(const std::string& shadingMode) { mShadingMode = shadingMode; }
	void setName(const std::string& name) { mMaterialName = name; }
	void setAnisotropy(const float& anisotropy) { mMapLightAnisotropy = anisotropy; }
	void setBlending(const std::string& blending) { mBlending = blending; }
	void setDiffuseColour(const VECTOR3& diffuseColour) { mColorDiffuse = diffuseColour; }
	void setDepthTest(const bool& depthTest) { mDepthTest = depthTest; }
	void setEmissiveColour(const VECTOR3& emissiveColour) { mColorEmissive = emissiveColour; }
	void setDepthWrite(const bool& depthWrite) { mDepthWrite = depthWrite; }
	void setOpacity(const float& opacity) { mOpacity = opacity; }
	void setAmbientColour(const VECTOR3& ambientColoutr) { mColorAmbient = ambientColoutr; }
	void setDiffuseTexture(const std::string& diffuseTexture) { mDiffuseTexture = diffuseTexture; }
	void setWrapU(const std::string& wrap) { mWrapU = wrap; }
	void setWrapV(const std::string& wrap) { mWrapV = wrap; }
	void setRepeatU(const int& repeat) { mRepeatU = repeat; }
	void setRepeatV(const int& repeat) { mRepeatV = repeat; }
	void setTransparent(const bool& transparent) { mTransparent = transparent; }
	void setTextureOffsetU(const float& offset) { mOffsetU = offset; }
	void setTextureOffsetV(const float& offset) { mOffsetV = offset; }
	void setUVLayer(const int& layer) { m_uvLayer = layer; }
protected:
	int mIndex;
	bool mWireframe;
	std::string mShadingMode;
	std::string mMaterialName;
	float mMapLightAnisotropy;
	std::string mBlending;
	VECTOR3 mColorDiffuse;
	bool mVisible;				// 此参数暂时无用
	bool mDepthTest;
	VECTOR3 mColorEmissive;
	bool mDepthWrite;
	float mOpacity;
	VECTOR3 mColorAmbient;
	std::string mDiffuseTexture;
	int mColor;					// 此参数暂时无用
	std::string mWrapU;
	std::string mWrapV;
	int mRepeatU;
	int mRepeatV;
	bool mTransparent;

	float mOffsetU;
	float mOffsetV;
	int m_uvLayer;
};

#endif