#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 1) uniform sampler2D texSampler;


layout(std140,binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec4 cameraFront;
    float time;
} ubo;


void main() {
    
    vec3 halfwayVector = (vec3(ubo.dirLight) + vec3(ubo.cameraFront) );
    float light = clamp(pow(dot(halfwayVector, fragNormal),5), 0.0, 1.0);


    outColor = texture(texSampler , fragTexCoord)* clamp(light +0.6,0.0,1.0);
    //outColor = vec4(fragNormal,1.0);
}
