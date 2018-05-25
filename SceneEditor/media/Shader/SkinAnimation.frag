#version 100
precision mediump float;

uniform sampler2D uSampler;
uniform sampler2D alphaSampler;
uniform vec3 uFogColor;
uniform float fAlpha;
uniform int hasAlphaTexture;

varying vec2 oTexCoord;
varying float fFogCoef;

void main()
{	
	vec4 diffuseColor = texture2D(uSampler, oTexCoord);
	if(hasAlphaTexture > 0)
	{
		vec4 alphaColour = texture2D(alphaSampler, oTexCoord);
		diffuseColor.a = alphaColour.a;
	}
	if(diffuseColor.a < 0.1)
            discard;
	diffuseColor.a = diffuseColor.a * fAlpha;
	vec4 fogColor = vec4(uFogColor,1.0);
	gl_FragColor = fFogCoef * fogColor + ( 1.0 - fFogCoef ) * diffuseColor;
}

