#version 100

attribute vec3 vPosition;

uniform mat4 uMVPMatrix;

void main()
{
	gl_Position = uMVPMatrix * vec4(vPosition, 1.0);
}