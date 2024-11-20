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
    vec4 data;
    vec4 grassBlade[ ];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
void main() {
    
    vec3 pos = vec3( grassBlade[gl_InstanceIndex].x + inPosition.x, 
                     grassBlade[gl_InstanceIndex].y + inPosition.y, 
                     grassBlade[gl_InstanceIndex].z + inPosition.z
                 );

    float theta = grassBlade[gl_InstanceIndex].w;
    mat3 rotation = mat3( cos(theta), -sin(theta), 0,
                          sin(theta),  cos(theta), 0,
                          0,           0,          1
                    );

    vec3 posRota = rotation * pos;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(posRota,1.0f);

    fragColor = inColor;    
    fragTexCoord = inTexCoord;
}