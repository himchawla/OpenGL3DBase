#version 460 core

in vec4 worldPos;
in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;


uniform vec3 cameraPos;

out vec4 color;



void main()
{

    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(fragPos - cameraPos);
    vec3 reflectDir = reflect(viewDir, norm);

    color  = vec4(reflectDir, 1.0);


    float d = distance(worldPos.xyz, cameraPos);
    float lerp = (d - 5.0f)/10.f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);

}