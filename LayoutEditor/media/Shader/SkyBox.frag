#version 100
precision mediump float;

uniform sampler2D uSampler;
uniform vec3 uFogColor;
uniform float fFogCoef;

varying vec2 oTexCoord;

void main()
{	
	vec4 diffuseColor = texture2D(uSampler, oTexCoord);
	vec4 fogColor = vec4(uFogColor, 1.0);
	gl_FragColor = fFogCoef * fogColor + (1.0 - fFogCoef) * diffuseColor;
}

