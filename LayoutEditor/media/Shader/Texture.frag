#version 100
precision mediump float;

varying vec2 oTexCoord;

uniform sampler2D uSampler;
uniform sampler2D uSamplerMask;
uniform float fAlpha;
uniform vec3 vColour;
uniform int blendMode;
uniform vec3 vHSLOffset;
uniform int hasMask;
uniform float fArc;
uniform float fLinearDodge;

float adjustAngle360(float radianAngle)
{
	// 如果小于0,则循环加上PI * 2
	while (radianAngle < 0.0)
	{
		radianAngle += 3.1415926 * 2.0;
	}
	// 如果大于PI * 2,则循环减去PI * 2
	while (radianAngle > 3.1415926 * 2.0)
	{
		radianAngle -= 3.1415926 * 2.0;
	}
	return radianAngle;
}

float getAngleBetweenVector(vec3 v1, vec3 v2)
{
	vec3 curVec1 = normalize(v1);
	vec3 curVec2 = normalize(v2);
	float dotValue = dot(curVec1, curVec2);
	dotValue = clamp(dotValue, 1.0, -1.0);
	return acos(dotValue);
}

float getAngleFromVectorToVector(vec2 from, vec2 to)
{
	vec3 fromVec3 = vec3(from.x, 0.0, from.y);
	vec3 toVec3 = vec3(to.x, 0.0, to.y);
	float angle = getAngleBetweenVector(fromVec3, toVec3);
	// 根据叉乘后向量的方向来决定角度处理
	vec3 crossVec = cross(fromVec3, toVec3);
	if (crossVec.y > 0.0)
	{
		angle = -angle;
	}
	angle = adjustAngle360(angle);
	return angle;
}

float HueToRGB(float v1, float v2, float vH)
{
	//if (vH < 0.0)
	//{
	//	vH += 1.0;
	//}
	//if (vH > 1.0) 
	//{
	//	vH -= 1.0;
	//}
	float s = sign(vH * vH - vH);	// 判断H是否小于0或者大于1(返回1), 大于等于0且小于等于1(返回0, -1)
	s = clamp(s, 0.0, 1.0);
	vH -= sign(vH) * s;
	if (6.0 * vH < 1.0) 
	{
		return v1 + (v2 - v1) * 6.0 * vH;
	}
	else if (2.0 * vH < 1.0) 
	{
		return v2;
	}
	else if (3.0 * vH < 2.0) 
	{
		return v1 + (v2 - v1) * (2.0 / 3.0 - vH ) * 6.0;
	}
	else
	{
		return v1;
	}
}

// rgb转换为色相(H),饱和度(S),亮度(L)
// HSL和RGB的范围都是0-1
vec3 RGBtoHSL(vec3 rgb)
{
	float minRGB = min(min(rgb.r, rgb.g), rgb.b);
	float maxRGB = max(max(rgb.r, rgb.g), rgb.b);
	float delta = maxRGB - minRGB;

	float H = 0.0;
	float S = 0.0;
	float L = (maxRGB + minRGB) / 2.0;
	// 如果三个分量的最大和最小相等,则说明该颜色是灰色的,灰色的色相和饱和度都为0
	if(delta > 0.0)
	{
		if (L < 0.5) 
		{
			S = delta / (maxRGB + minRGB);
		}
		else           
		{
			S = delta / (2.0 - maxRGB - minRGB);
		}

		float delR = ((maxRGB - rgb.r) / 6.0 + delta / 2.0) / delta;
		float delG = ((maxRGB - rgb.g) / 6.0 + delta / 2.0) / delta;
		float delB = ((maxRGB - rgb.b) / 6.0 + delta / 2.0) / delta;

		if (rgb.r == maxRGB) 
		{
			H = delB - delG;
		}
		else if (rgb.g == maxRGB) 
		{
			H = 1.0 / 3.0 + delR - delB;
		}
		else if (rgb.b == maxRGB) 
		{
			H = 2.0 / 3.0 + delG - delR;
		}
		
		//if (H < 0.0)
		//{
		//	H += 1.0;
		//}
		//if (H > 1.0) 
		//{
		//	H -= 1.0;
		//}
		float s = sign(H * H - H);	// 判断H是否小于0或者大于1(返回1), 大于等于0且小于等于1(返回0, -1)
		s = clamp(s, 0.0, 1.0);
		H -= sign(H) * s;
	}
	return vec3(H, S, L);
}

// 色相(H),饱和度(S),亮度(L),转换为rgb
// HSL和RGB的范围都是0-1
vec3 HSLtoRGB(vec3 hsl)
{
	vec3 rgb;
	float H = hsl.r;
	float S = hsl.g;
	float L = hsl.b;
	if (S == 0.0)				// HSL from 0 to 1
	{
		rgb.r = L;				// RGB results from 0 to 255
		rgb.g = L;
		rgb.b = L;
	}
	else
	{
		float var2;
		if (L < 0.5) 
		{
			var2 = L * (1.0 + S);
		}
		else           
		{
			var2 = L + S - (S * L);
		}

		float var1 = 2.0 * L - var2;
		rgb.r = HueToRGB(var1, var2, H + 1.0 / 3.0);
		rgb.g = HueToRGB(var1, var2, H);
		rgb.b = HueToRGB(var1, var2, H - 1.0 / 3.0);
	}
	return rgb;
}

void main()
{
	vec2 zeroPos = vec2(0.5, 1.0);
	vec2 centerPos = vec2(0.5, 0.5);
	if(fArc <= 0.0 || (fArc < 3.1415926 * 2.0 && getAngleFromVectorToVector(oTexCoord - centerPos, zeroPos - centerPos) >= fArc))
	{
		discard;
	}
	else
	{
		vec4 colour = texture2D(uSampler, oTexCoord);
		// 如果存在遮挡纹理,则采集遮挡纹理的透明分量设置到纹理颜色的透明值
		if(hasMask > 0)
		{
			vec4 maskColour = texture2D(uSamplerMask, oTexCoord);
			colour.a = maskColour.a;
		}
		colour.a = colour.a * fAlpha;
		// 0是add,直接进行颜色相加
		if(blendMode == 0)
		{
			colour.r = colour.r + vColour.r;
			colour.g = colour.g + vColour.g;
			colour.b = colour.b + vColour.b;
		}
		// 1是multi,颜色相乘
		else if(blendMode == 1)
		{
			colour.r = colour.r * vColour.r;
			colour.g = colour.g * vColour.g;
			colour.b = colour.b * vColour.b;
		}
		// 2是replace,颜色替换
		else if(blendMode == 2)
		{
			colour.r = vColour.r;
			colour.g = vColour.g;
			colour.b = vColour.b;
		}
		// 颜色值限定在0-1之间
		colour.r = clamp(colour.r, 0.0, 1.0);
		colour.g = clamp(colour.g, 0.0, 1.0);
		colour.b = clamp(colour.b, 0.0, 1.0);
		
		if(length(vHSLOffset) > 0.0)
		{
			// 转换到HSL颜色空间,再做HSL计算偏移
			vec3 HSL = RGBtoHSL(vec3(colour.r, colour.g, colour.b));
			HSL += vHSLOffset;
			
			// 转回RGB空间,并转换到0-1之间
			vec3 rgb = HSLtoRGB(HSL);
			
			// 颜色值限定在0-1之间
			colour.r = clamp(rgb.r, 0.0, 1.0);
			colour.g = clamp(rgb.g, 0.0, 1.0);
			colour.b = clamp(rgb.b, 0.0, 1.0);
		}
		
		// 线性减淡时,需要自己根据透明度重新计算颜色值
		colour.r *= mix(1.0, colour.a, fLinearDodge * 1.0);
		colour.g *= mix(1.0, colour.a, fLinearDodge * 1.0);
		colour.b *= mix(1.0, colour.a, fLinearDodge * 1.0);
		gl_FragColor = colour;
	}
}