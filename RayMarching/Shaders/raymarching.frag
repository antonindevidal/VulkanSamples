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

const int MAX_MARCHING_STEPS = 80;
const float MIN_DIST = 0.0;
const float MAX_DIST = 50.0;
const float PRECISION = 0.0001;

struct Surface {
    float sd;
    vec3 col;
};

Surface sdfSphere(vec3 p, float r, vec3 col)
{
    float d = length(p) - r; 
    return Surface(d,col);
}

Surface sdfBox(vec3 p, vec3 b, vec3 col)
{
    vec3 q = abs(p) - b;
    float d = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    return Surface(d, col);
}

Surface sdfPlane(vec3 p, vec3 col) {
  float d = p.z + 5.;
  return Surface(d, col);
}

Surface minSurf(Surface object, Surface other)
{
    if( object.sd > other.sd)
    {
        return other;
    }
    return object;
}

Surface map(vec3 p)
{
    vec3 spherePos = vec3(sin(ubo.time) * 3.0,0.0,0.0);
    vec3 boxPos = vec3(0.0,0.0,0.0);
    vec3 floorPos = vec3(0.0,0.0,-5.0);

    Surface sphere = sdfSphere(p - spherePos, 1.0, vec3(1.0,0.0,0.0)); 
    Surface box = sdfBox(p - boxPos,vec3(0.75),vec3(0.0,1.0,0.0));
    Surface plane = sdfPlane(p, vec3( + 0.7*mod(floor(p.x) + floor(p.y), 2.0)));

    Surface temp = minSurf(box,sphere);
    return minSurf(temp,plane);
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

    Surface d;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++)
    {
        vec3 p = ro + rd * t; // Position along ray
        d = map(p); // Max safe distance
        t += d.sd;

        if( d.sd < PRECISION || t > MAX_DIST) break;
    }
    d.sd = t;

    if(d.sd > MAX_DIST)
    {
        color = vec3( 1.0,1.0,1.0);
    }
    else
    {
        color = d.col;
    }

    outColor = vec4(color,1.0);    
}