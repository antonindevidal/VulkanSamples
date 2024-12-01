#version 450

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec4 cameraFront;    
    vec4 cameraPos;
    vec4 windowData;
    float time;
} ubo;

layout(location = 0) in vec3 uvw;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform samplerCube texSampler;

void main() {
    outColor = texture(texSampler,uvw);
}