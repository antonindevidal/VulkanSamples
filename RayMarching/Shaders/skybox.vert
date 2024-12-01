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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 uvw;

void main() {
    
    mat4 no_translation_view = ubo.view;
    no_translation_view[3] = vec4(0.0, 0.0, 0.0, 1.0); // Skybox doesn't move with camera;

    mat4 rotationZup = mat4(1.0, 0.0,  0.0, 0.0,
                            0.0, 0.0,  1.0, 0.0,
                            0.0, -1.0, 0.0, 0.0,
                            0.0, 0.0,  0.0, 1.0);

    gl_Position = ubo.proj * no_translation_view * rotationZup* vec4(inPosition, 1.0);

    uvw = inPosition;
}