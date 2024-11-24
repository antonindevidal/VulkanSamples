#version 450

layout(location = 0) in vec3 uvw;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform samplerCube texSampler;

void main() {
    outColor = texture(texSampler,uvw);
}