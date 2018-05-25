#version 100
precision mediump float;

uniform sampler2D uSampler;
uniform sampler2D uShadowSampler;
uniform vec3 uFogColor;

varying vec2 oTexCoord;
varying float fFogCoef;
varying vec4 depthCoord;

void main()
{	
	vec4 diffuseColor = texture2D(uSampler, oTexCoord);
	if(diffuseColor.a < 0.1)
	{
		discard;
	}
	vec4 fogColor = vec4(uFogColor, 1.0);
	gl_FragColor = mix(diffuseColor, fogColor, fFogCoef);
	
	float inverseW = 1.0 / depthCoord.w;
	vec4 shadowCoord = depthCoord * inverseW;
	vec2 shadowMapCoord = vec2(shadowCoord.x * 0.5 + 0.5, shadowCoord.y * 0.5 + 0.5);
	vec4 shadowMapColor = texture2D(uShadowSampler, shadowMapCoord);
	float sceneDepth = shadowCoord.z;
	float shadowDepth = shadowMapColor.r;
	float shadowCoef = 1.0;
	if(shadowDepth < sceneDepth)
	{
		shadowCoef = 0.6;
	}
	else
	{
		shadowCoef = 1.0;
	}
	gl_FragColor = vec4(shadowCoef * gl_FragColor.r, shadowCoef * gl_FragColor.g, shadowCoef * gl_FragColor.b, gl_FragColor.a);
}

