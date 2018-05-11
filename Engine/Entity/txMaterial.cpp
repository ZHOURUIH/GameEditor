#include "txMaterial.h"

txMaterial::txMaterial()
{
	mIndex = 0;
	mWireframe = false;
	mShadingMode = "lambert";
	mMapLightAnisotropy = 1.0f;
	mBlending = "NormalBlending";
	mVisible = true;
	mDepthTest = true;
	mDepthWrite = true;
	mOpacity = 1.0f;
	mColor = 0;
	mWrapU = "RepeatWrapping";
	mWrapV = "RepeatWrapping";
	mRepeatU = 1;
	mRepeatV = 1;
	mTransparent = false;

	mOffsetU = 0.0f;
	mOffsetV = 0.0f;
	m_uvLayer = 0;
}

void txMaterial::operator = (const txMaterial& that)
{
	mIndex = that.mIndex;
	mWireframe = that.mWireframe;
	mShadingMode = that.mShadingMode;
	mMaterialName = that.mMaterialName;
	mMapLightAnisotropy = that.mMapLightAnisotropy;
	mBlending = that.mBlending;
	mColorDiffuse = that.mColorDiffuse;
	mVisible = that.mVisible;
	mDepthTest = that.mDepthTest;
	mColorEmissive = that.mColorEmissive;
	mDepthWrite = that.mDepthWrite;
	mOpacity = that.mOpacity;
	mColorAmbient = that.mColorAmbient;
	mDiffuseTexture = that.mDiffuseTexture;
	mColor = that.mColor;
	mWrapU = that.mWrapU;
	mWrapV = that.mWrapV;
	mRepeatU = that.mRepeatU;
	mRepeatV = that.mRepeatV;
	mTransparent = that.mTransparent;

	mOffsetU = that.mOffsetU;
	mOffsetV = that.mOffsetV;
	m_uvLayer = that.m_uvLayer;
}