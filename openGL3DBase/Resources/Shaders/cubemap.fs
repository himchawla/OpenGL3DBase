#version 460 core

in vec3 fragTexCoords;




out vec4 color;

uniform samplerCube cubeSampler;

void main()
{
    color  = texture(cubeSampler, fragTexCoords);
}