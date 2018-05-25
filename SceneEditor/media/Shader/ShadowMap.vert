#version 100

uniform mat4 uMVPMatrix;
attribute vec3 vPosition;
attribute vec2 vTexCoord;

varying vec2 oTexCoord;
varying float depth;

void main()
{
    oTexCoord = vTexCoord;
    gl_Position = uMVPMatrix * vec4(vPosition, 1.0);
    depth = gl_Position.z / gl_Position.w;
}

