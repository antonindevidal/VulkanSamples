#version 450

struct GrassBlade
{
    vec4 position;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(std140, binding = 0, set = 1) readonly buffer GrassBladeSSBOIn {
   GrassBlade grassBlade[ ];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * (grassBlade[gl_InstanceIndex].position + vec4(inPosition,1.0f));
    fragColor = inColor;    
    fragTexCoord = inTexCoord;
}