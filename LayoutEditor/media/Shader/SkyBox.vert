#version 100

uniform mat4 uVPMatrix;

attribute vec3 vPosition;
attribute vec2 vTexCoord;

varying vec2 oTexCoord;

void main()
{
    oTexCoord = vTexCoord;
    gl_Position = uVPMatrix * vec4(vPosition, 1.0);
}
