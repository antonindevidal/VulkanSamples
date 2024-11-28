#version 450

layout(std140, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec4 cameraFront;
    vec4 cameraPos;
    float time;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


float sdfSphere(vec3 p, float r)
{
    return length(p) - r ; 
}

float sdfBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float map(vec3 p)
{
    vec3 spherePos = vec3(sin(ubo.time) * 3.0,0.0,0.0);
    vec3 boxPos = vec3(0.0,0.0,0.0);

    float sphere = sdfSphere(p - spherePos, 1.0) ; 
    float box = sdfBox(p-boxPos,vec3(0.75));
    
    return min(box,sphere);
}


void main() {

    float fov = 45;
    vec2 res = vec2(600,600);
    float aspect = res.x/res.y;

    vec2 uv = vec2((fragTexCoord*2)-1);

    vec3 ro = vec3(ubo.cameraPos); //Ray origin
 
    // Ray direction
    vec2 offsets = uv * tan(fov/2.0);
    vec3 rayFront = normalize(vec3(ubo.cameraFront));
    vec3 rayRight = normalize(cross(rayFront, normalize(vec3(0.0,0.0,1.0))));
    vec3 rayUp = cross(rayRight, rayFront);
    vec3 rayDir = rayFront + rayRight * offsets.x + rayUp * offsets.y;
    vec3 rd = normalize(rayDir);


    vec3 color = vec3(0.0);

    float t = 0.0; // Distance travelled


    for (int i = 0; i < 80; i++)
    {
        vec3 p = ro + rd * t; // Position along ray
        float d = map(p); // Max safe distance
        t += d;

        if( d < 0.001 || d > 100.0) break;
    }

    color = vec3( t*0.2);

    outColor = vec4(color,1.0);    
}