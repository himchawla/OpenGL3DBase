#version 460 core

uniform float iTime;

uniform sampler2D gSampler;


 out vec4 outputColor;

 in vec2 fragCoord;
void main()
{
	// distance from center of image, used to adjust blur
	vec2 uv = fragCoord.xy / vec2(1000.0f,1000.0f);
	float d = length(uv - vec2(0.5,0.5));
	
	// blur amount
	float blur = 0.0;	
	blur = (1.0 + sin(iTime*6.0)) * 0.5;
	blur *= 1.0 + sin(iTime*16.0) * 0.5;
	blur = pow(blur, 3.0);
	blur *= 0.05;
	// reduce blur towards center
	blur *= d;
	
	// final color
    vec3 col;
    col.r = texture(gSampler, vec2(uv.x+blur,uv.y) ).r;
    col.g = texture(gSampler, uv ).g;
    col.b = texture(gSampler, vec2(uv.x-blur,uv.y) ).b;
	
	// scanline
	float scanline = sin(uv.y*800.0)*0.04;
	col -= scanline;
	
	// vignette
	col *= 1.0 - d * 0.5;
	outputColor = vec4(col,1.0);
}
