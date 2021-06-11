#version 430 core

layout (vertices = 3) out;
void main() 
{
	gl_TessLevelOuter[0] =0;
	gl_TessLevelOuter[1] =0;
	gl_TessLevelOuter[2] =0;
	gl_TessLevelInner[0] =0;
	gl_out[gl_InvocationID].gl_Position = 
	gl_in[gl_InvocationID].gl_Position;
}