#version 100

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform vec3 uLightDirection;
uniform vec3 uViewPosition;
uniform float uNs;
uniform float uNearD;
uniform float uFarD;
uniform float fAlpha;

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying float fDiffuse;
varying float fSpec;
varying float fFogCoef;
varying vec2 oTexCoord;

void main()
{
    vec3 vPos = vec3(uMVMatrix * vec4(vPosition, 1.0));
	float fDistance = length(vPos);
	fFogCoef = (fDistance - uNearD) / (uFarD - uNearD);
	fFogCoef = clamp(fFogCoef, 0.0, 1.0);
    oTexCoord = vTexCoord;
    gl_Position = uMVPMatrix * vec4(vPosition, 1.0);
}
