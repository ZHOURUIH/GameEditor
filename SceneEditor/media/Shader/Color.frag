#version 100
precision mediump float;

uniform float fAlpha;

varying vec4 oColor;

void main()
{
	gl_FragColor = oColor;
	gl_FragColor.a = gl_FragColor.a * fAlpha;
}

