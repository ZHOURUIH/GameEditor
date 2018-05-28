#version 100

attribute vec3 vPosition;
attribute vec2 vTexCoord;

uniform mat4 uMVPMatrix;

varying vec2 oTexCoord;

void main()
{
	gl_Position = uMVPMatrix * vec4(vPosition, 1.0);
	oTexCoord = vTexCoord;
}

