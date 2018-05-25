#version 100

attribute vec3 vPosition;

uniform mat4 uMVPMatrix;
uniform vec4 uColor;
uniform float fAlpha;

varying vec4 oColor;

void main()
{
	gl_Position = uMVPMatrix * vec4(vPosition, 1.0);
	oColor = uColor;
}

