#version 100
precision mediump float;

uniform sampler2D uSampler;
uniform sampler2D uShadowSampler;
uniform float uKs;
uniform float uKd;
uniform float uKa;
uniform vec3 uLightColor;
uniform vec3 uFogColor;
uniform float uInc;
uniform float vInc;

varying float fDiffuse;
varying float fSpec;
varying vec2 oTexCoord;
varying float fFogCoef;
varying vec4 depthCoord;

void main()
{	
	vec2 uvInc;
	vec4 diffuseColor;
	if(uInc > 0.001 || vInc > 0.001)
	{
		uvInc.x = oTexCoord.x + uInc;
		uvInc.y = oTexCoord.y + vInc;
		diffuseColor = texture2D(uSampler, uvInc);
	}
	else
	{
		diffuseColor = texture2D(uSampler, oTexCoord);
	}

	if(diffuseColor.a < 0.1)
            discard;
	vec4 fogColor = vec4(uFogColor,1.0);
	gl_FragColor = fFogCoef * fogColor + ( 1.0 - fFogCoef ) * diffuseColor;

	vec4 shadowCoord = vec4(depthCoord.x/depthCoord.w,depthCoord.y/depthCoord.w,depthCoord.z/depthCoord.w,depthCoord.w/depthCoord.w);
	vec2 shadowMapCoord = vec2(shadowCoord.x * 0.5 + 0.5,shadowCoord.y * 0.5 + 0.5);

	vec4 shadowMapColor = texture2D(uShadowSampler, shadowMapCoord);
	float sceneDepth = shadowCoord.b;
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
	gl_FragColor = vec4(shadowCoef * gl_FragColor.r,shadowCoef * gl_FragColor.g,shadowCoef * gl_FragColor.b,gl_FragColor.a);
}

