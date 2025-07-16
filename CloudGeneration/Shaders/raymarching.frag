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
    float reflection;
};

Surface sdfSphere(vec3 p, float r, vec3 col)
{
    float d = length(p) - r; 
    return Surface(d,col,0.8);
}

Surface sdfBox(vec3 p, vec3 b, vec3 col)
{
    vec3 q = abs(p) - b;
    float d = length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    return Surface(d, col,0.8);
}

Surface sdfPlane(vec3 p, vec3 pos, vec3 col) {
  float d = p.z - pos.z;
  return Surface(d, col,0.0);
}

Surface minSurf(Surface object, Surface other)
{
    if( object.sd > other.sd)
    {
        return other;
    }
    return object;
}

Surface opSmoothUnion( Surface d1, Surface d2, float k )
{
    Surface s;
    float h = clamp( 0.5 + 0.5*(d2.sd-d1.sd)/k, 0.0, 1.0 );
    s.sd = mix( d2.sd, d1.sd, h ) - k*h*(1.0-h);
    s.col = mix( d2.col, d1.col, h ) - k*h*(1.0-h);
    s.reflection = mix( d2.reflection, d1.reflection, h );
    return s;
}

Surface map(vec3 p)
{
    vec3 spherePos = vec3(sin(ubo.time) * 3.0,0.0,0.0);
    vec3 boxPos = vec3(0.0,0.0,0.0);
    //vec3 floorPos = vec3(0.0,0.0,-3.0);

    Surface sphere = sdfSphere(p - spherePos, 1.0, vec3(1.0,0.0,0.0)); 
    Surface box = sdfBox(p - boxPos,vec3(0.75),vec3(0.0,1.0,0.0));
    //Surface plane = sdfPlane(p, floorPos, vec3( + 0.7*mod(floor(p.x) + floor(p.y), 2.0)));

    Surface temp = opSmoothUnion(box,sphere,1.0);
    return temp;//minSurf(temp,plane);
}

vec3 calcNormal(vec3 pos )
{
    vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*map( pos + e.xyy ).sd.x + 
					  e.yyx*map( pos + e.yyx ).sd.x + 
					  e.yxy*map( pos + e.yxy ).sd.x + 
					  e.xxx*map( pos + e.xxx ).sd.x );
}
float calcShadow(vec3 ro, vec3 rd, float mint, float maxt, float k)
{
    float res = 1.0;
    float t = mint;
    for( int i=0; i<256 && t<maxt; i++ )
    {
        float h = map(ro + rd*t).sd;
        if( h<0.001 )
            return 0.0;
        res = min( res, k*h/t );
        t += h;
    }
    return res;
}

void main() {

    float fov = ubo.windowData.z;
    vec2 res = vec2(ubo.windowData.x,ubo.windowData.y);
    float aspect = res.x/res.y;

    vec2 uv = vec2((fragTexCoord*2)-1);
    uv.x *= aspect;

    vec3 ro = vec3(ubo.cameraPos); //Ray origin
 
    // Ray direction
    vec2 offsets = uv * tan(fov/2.0);
    vec3 rayFront = normalize(vec3(ubo.cameraFront));
    vec3 rayRight = normalize(cross(rayFront, normalize(vec3(0.0,0.0,1.0))));
    vec3 rayUp = cross(rayRight, rayFront);
    vec3 rayDir = rayFront + rayRight * offsets.x + rayUp * offsets.y;
    vec3 rd = normalize(rayDir);


    vec4 color = vec4(0.0);

    float t = 0.0; // Distance travelled

    Surface d;
    vec3 p;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++)
    {
        p = ro + rd * t; // Position along ray
        d = map(p); // Max safe distance
        t += d.sd;

        if( d.sd < PRECISION || t > MAX_DIST) break;
    }
    d.sd = t;


    if(d.sd > MAX_DIST)
    {
        color = vec4( 1.0,1.0,1.0,0.0);
    }
    else
    {
        // Color calculation
        vec3 norm = calcNormal(p);

        mat3 rotationZup = mat3(1.0, 0.0,  0.0,
                                0.0, 0.0,  1.0,
                                0.0, 1.0, 0.0);


        // Blinn Phong
        float diffuse = max(0.0,dot(-vec3(ubo.dirLight),norm));
        vec3 hv = normalize(vec3(-ubo.dirLight) - rd);
        float specular = pow(max(0.0,dot(hv,norm)),64);

        float shadow = calcShadow(p,vec3(-ubo.dirLight),0.02,10,32);
        //color = vec4(mix(d.col, cubemapReflectionColor, d.reflection) * (0.2 + diffuse * shadow  + specular) ,1);

        color = vec4(d.col * (0.2 + diffuse * shadow  + specular) ,1);
    }

    outColor = color;    
}