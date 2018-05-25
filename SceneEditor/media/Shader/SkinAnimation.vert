#version 100

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform float uNearD;
uniform float uFarD;
uniform float fAlpha;
uniform mat4  boneMatrices[50];

attribute vec3 vPosition;
attribute vec2 vTexCoord;
attribute vec4 Weight;
attribute vec4 Bone;

varying float fFogCoef;
varying vec2 oTexCoord;

void main()
{
	// calculate the skin position
    mat4 transformMat1 =  boneMatrices[int(min(Bone[0], 49.0))];
    mat4 transformMat2 =  boneMatrices[int(min(Bone[1], 49.0))];
    mat4 transformMat3 =  boneMatrices[int(min(Bone[2], 49.0))];
    mat4 transformMat4 =  boneMatrices[int(min(Bone[3], 49.0))];

    vec4 skinPosition;
	skinPosition +=	transformMat1 * vec4(vPosition, 1.0) * Weight[0];
	skinPosition +=	transformMat2 * vec4(vPosition, 1.0) * Weight[1];
    skinPosition += transformMat3 * vec4(vPosition, 1.0) * Weight[2];
	skinPosition += transformMat4 * vec4(vPosition, 1.0) * Weight[3];

    vec3 vPos = vec3(uMVMatrix * skinPosition);
    float fDistance = length(vPos);
	fFogCoef = (fDistance - uNearD) / (uFarD - uNearD);
	fFogCoef = clamp(fFogCoef, 0.0, 1.0);
    oTexCoord = vTexCoord;
    gl_Position = uMVPMatrix * skinPosition;
}