#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;
out vec4 outColor;

uniform mat4 vp;
uniform vec3 vQuad1, vQuad2;


void main() 
{ 
	outColor = vec4(1.0f,1.0f,1.0f,1.0f);

	float size = 0.1;

	vec3 p1 = gl_in[0].gl_Position.xyz + (-vQuad1-vQuad2)* size;
	gl_Position = vp * vec4(p1, 1.0f);EmitVertex();
	vec3 p2 = gl_in[0].gl_Position.xyz + (-vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p2, 1.0f);EmitVertex();
	vec3 p3 = gl_in[0].gl_Position.xyz + (vQuad1-vQuad2)* size;
	gl_Position = vp * vec4(p3, 1.0f);EmitVertex();
	vec3 p4 = gl_in[0].gl_Position.xyz + (vQuad1+vQuad2)* size;
	gl_Position = vp * vec4(p4, 1.0f);EmitVertex();
	
	
	EndPrimitive();
}