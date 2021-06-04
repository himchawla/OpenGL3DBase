#version 460 core

layout (location = 0) in vec3 position;

out vec3 fragTexCoords;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	gl_Position = MVP * vec4(position, 1.0f);
	
	fragTexCoords = position;
}