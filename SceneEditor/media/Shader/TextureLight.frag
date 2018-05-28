#version 100
precision mediump float;

uniform sampler2D uSampler;
uniform sampler2D alphaSampler;
uniform float uKs;
uniform float uKd;
uniform float uKa;
uniform vec3 uLightColor;
uniform vec3 uFogColor;
uniform float uInc;
uniform float vInc;
uniform float fAlpha;
uniform int hasAlphaTexture;

varying float fDiffuse;
varying float fSpec;
varying vec2 oTexCoord;
varying float fFogCoef;

void main()
{
	vec4 diffuseColor;
	if(uInc > 0.001 || vInc > 0.001)
	{
		vec2 uvInc;
		uvInc.x = oTexCoord.x + uInc;
		uvInc.y = oTexCoord.y + vInc;
		diffuseColor = texture2D(uSampler, uvInc);
		if(hasAlphaTexture > 0)
		{
			vec4 alphaColour = texture2D(alphaSampler, uvInc);
			diffuseColor.a = alphaColour.a;
		}
	}
	else
	{
		diffuseColor = texture2D(uSampler, oTexCoord);
		if(hasAlphaTexture > 0)
		{
			vec4 alphaColour = texture2D(alphaSampler, oTexCoord);
			diffuseColor.a = alphaColour.a;
		}
	}

	if(diffuseColor.a < 0.1)
            discard;
	diffuseColor.a = diffuseColor.a * fAlpha;
	vec4 fogColor = vec4(uFogColor, 1.0);
	gl_FragColor = fFogCoef * fogColor + ( 1.0 - fFogCoef ) * diffuseColor;
}

