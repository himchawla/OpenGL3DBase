#version 460 core

layout(location = 0) out vec4 outputColor;

smooth in vec3 ioVertexColor;
smooth in vec2 ioVertexTexCoord;
smooth in float ioHeight;

uniform sampler2D gSampler;
uniform vec4 color;

void main()
{
    
		outputColor = color;
}