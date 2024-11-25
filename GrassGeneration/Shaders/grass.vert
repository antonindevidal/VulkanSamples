#version 450

struct GrassBlade
{
    vec4 position;
    vec4 data;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    float time;
} ubo;


layout(std140, binding = 0, set = 1) readonly buffer GrassBladeSSBOIn {
    vec4 data;
    vec4 directionnalLight;
    GrassBlade grassBlade[ ];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;

void main() {
    float heightFactor = grassBlade[gl_InstanceIndex].data.x;
    float bladeInitialRotation= grassBlade[gl_InstanceIndex].position.w;

    vec3 bladePos = vec3(inPosition.x, inPosition.y, inPosition.z);
    vec3 bladeOffset = vec3(grassBlade[gl_InstanceIndex].position.x, grassBlade[gl_InstanceIndex].position.y, grassBlade[gl_InstanceIndex].position.z + bladePos.z * heightFactor);
    


    float curvFactor = 0.5 * bladePos.z * heightFactor ;
    mat3 curvature = mat3(cos(curvFactor), 0, -sin(curvFactor),
                          0              , 1, 0,
                          sin(curvFactor), 0, cos(curvFactor)
                    );

    mat3 rotation = mat3( cos(bladeInitialRotation), -sin(bladeInitialRotation), 0,
                          sin(bladeInitialRotation),  cos(bladeInitialRotation), 0,
                          0,           0,          1
                    );

    vec3 bladePosWindEffect = rotation * curvature * bladePos;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(bladePosWindEffect + bladeOffset,1.0f);

    fragColor = inColor;    
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
}