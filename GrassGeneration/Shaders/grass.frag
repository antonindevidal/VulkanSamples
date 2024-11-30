#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragVertPos;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform sampler2D texSampler;


layout(std140,binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec4 cameraFront;
    vec4 cameraPosition;
    float time;
} ubo;


void main() 
{
    float diffuse = max(0.0,dot(vec3(ubo.dirLight),fragNormal));


    vec3 viewDir = normalize(vec3(ubo.cameraPosition) - fragVertPos);
    vec3 hv = normalize(vec3(ubo.dirLight) + viewDir);
    float specular = pow(max(0.0,dot(hv,fragNormal)),16);
    

    outColor = texture(texSampler , fragTexCoord)* ( 0.4 + diffuse + specular);
    //outColor = vec4(fragNormal,1.0);
}
