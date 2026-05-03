#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform bool doAA;
uniform ivec2 ScreenResolution;
uniform float scale;

float maxLumaThreshold = 0.125f;
float minLumaThreshold = 0.0312f;
float u_mulReduce = 0.5f;
float u_minReduce = 1./128.f;
float u_maxSpan = 8f;


vec3 computeAA(void)
{
    vec3 rgbM = texture(screenTexture, TexCoords).rgb;
	vec2 inverseVP = vec2(1.0 / ScreenResolution.x, 1.0 / ScreenResolution.y) * 1;
	// vec2 inverseVP = vec2(1.0 / 1920 * 2, 1.0 / 1080 * 2);


	// Sampling neighbour texels. Offsets are adapted to OpenGL texture coordinates. 
	vec3 rgbNW = textureOffset(screenTexture, TexCoords, ivec2(-1, 1)).rgb;
    vec3 rgbNE = textureOffset(screenTexture, TexCoords, ivec2(1, 1)).rgb;
    vec3 rgbSW = textureOffset(screenTexture, TexCoords, ivec2(-1, -1)).rgb;
    vec3 rgbSE = textureOffset(screenTexture, TexCoords, ivec2(1, -1)).rgb;

	// see http://en.wikipedia.org/wiki/Grayscale
	const vec3 toLuma = vec3(0.299, 0.587, 0.114);
	
	// Convert from RGB to luma.
	float lumaNW = dot(rgbNW, toLuma);
	float lumaNE = dot(rgbNE, toLuma);
	float lumaSW = dot(rgbSW, toLuma);
	float lumaSE = dot(rgbSE, toLuma);
	float lumaM = dot(rgbM, toLuma);

	// Gather minimum and maximum luma.
	float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	
	// If contrast is lower than a maximum threshold ...
	if (lumaMax - lumaMin <= max(minLumaThreshold, lumaMax * maxLumaThreshold))
		return rgbM;
	
	// Sampling is done along the gradient.
	vec2 samplingDirection;	
	samplingDirection.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    samplingDirection.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    // Sampling step distance depends on the luma: The brighter the sampled texels, the smaller the final sampling step direction.
    // This results, that brighter areas are less blurred/more sharper than dark areas.  
    float samplingDirectionReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * 0.25 * u_mulReduce, u_minReduce);

	// Factor for norming the sampling direction plus adding the brightness influence. 
	float minSamplingDirectionFactor = 1.0 / (min(abs(samplingDirection.x), abs(samplingDirection.y)) + samplingDirectionReduce);
    
    // Calculate final sampling direction vector by reducing, clamping to a range and finally adapting to the texture size. 
    samplingDirection = clamp(samplingDirection * minSamplingDirectionFactor, vec2(-u_maxSpan), vec2(u_maxSpan)) * inverseVP;
	
	// Inner samples on the tab.
	vec3 rgbSampleNeg = texture(screenTexture, TexCoords + samplingDirection * (1.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePos = texture(screenTexture, TexCoords + samplingDirection * (2.0/3.0 - 0.5)).rgb;

	vec3 rgbTwoTab = (rgbSamplePos + rgbSampleNeg) * 0.5;  

	// Outer samples on the tab.
	vec3 rgbSampleNegOuter = texture(screenTexture, TexCoords + samplingDirection * (0.0/3.0 - 0.5)).rgb;
	vec3 rgbSamplePosOuter = texture(screenTexture, TexCoords + samplingDirection * (3.0/3.0 - 0.5)).rgb;
	
	vec3 rgbFourTab = (rgbSamplePosOuter + rgbSampleNegOuter) * 0.25 + rgbTwoTab * 0.5;   
	
	// Calculate luma for checking against the minimum and maximum value.
	float lumaFourTab = dot(rgbFourTab, toLuma);

	vec3 res;
	
	// Are outer samples of the tab beyond the edge ... 
	if (lumaFourTab < lumaMin || lumaFourTab > lumaMax)
		res = rgbTwoTab; 
	else
		res = rgbFourTab;

	// res.r = 1.0;
	return res;
}


void main() {
	FragColor = vec4(TexCoords, 0, 1);
	// return;

    if (!doAA) {
        FragColor = texture(screenTexture, TexCoords);
        return;
    }

    // FragColor = 1 - texture(screenTexture, TexCoords);
    FragColor = vec4(computeAA(), 1.0);
}