#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec3 cameraFront;
    vec4 cameraPosition;
    float time;
} ubo;

void main() {
    vec3 halfwayVector = (vec3(ubo.dirLight) + ubo.cameraFront );

    float light = clamp(pow(dot(halfwayVector, -fragNormal),5), 0.0, 1.0);
    outColor = vec4(fragColor, 1.0f);
}