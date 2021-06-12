#version 430 core
//size of output patch & no. of times the TCS will be executed layout (vertices = 4) out; 

layout (vertices = 4) out; 

uniform float gOuter;
uniform float gInner;

void main() 
{
	if (gl_InvocationID == 0) 
	{
		gl_TessLevelInner[0] = gInner; 
		gl_TessLevelInner[1] = gInner;
								
		gl_TessLevelOuter[0] = gOuter; 
		gl_TessLevelOuter[1] = gOuter; 
		gl_TessLevelOuter[2] = gOuter; 
		gl_TessLevelOuter[3] = gOuter;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}