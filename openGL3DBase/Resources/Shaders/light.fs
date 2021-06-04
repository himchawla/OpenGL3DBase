#version 460 core


in vec3 fragPos;
in vec3 fragNormal;

in vec2 fragTexCoords;

in vec4 worldPos;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform float ambientStr = 0.1f;
uniform vec3 ambientColor = vec3(1.0f,1.0f,1.0f);
uniform vec3 lightColor = vec3(1.0f,1.0f,1.0f);
uniform vec3 lightPos = vec3(-2.0f,6.0f,3.0f);


uniform vec3 cameraPos;
uniform float lightSpecStr = 2.0f;
uniform float shininess = 64.0f;

void main()
{
 
 color  = vec4((texture(texture_diffuse1,fragTexCoords)));


    float d = distance(worldPos.xyz, cameraPos);
    float lerp = (d - 15.0f)/10.f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
}