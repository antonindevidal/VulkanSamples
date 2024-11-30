#version 450

struct GrassBlade
{
    vec4 position;
    vec4 data;
};

layout(std140,binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 dirLight;
    vec4 cameraFront;
    vec4 cameraPosition;
    float time;
} ubo;


layout(std140, binding = 0, set = 1) readonly buffer GrassBladeSSBOIn {
    vec4 data;
    vec4 wind;
    GrassBlade grassBlade[ ];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragVertPos;


float PI = 3.14;


// Might move that to its own compute shader that runs every frame
//Perlin noise from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 c){
	return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
	float unit = 1920/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}

float pNoise(vec2 p, int res){
	float persistance = .5;
	float n = 0.;
	float normK = 0.;
	float f = 4.;
	float amp = 1.;
	int iCount = 0;
	for (int i = 0; i<50; i++){
		n+=amp*noise(p, f);
		f*=2.;
		normK+=amp;
		amp*=persistance;
		if (iCount == res) break;
		iCount++;
	}
	float nf = n/normK;
	return nf*nf*nf*nf;
}


void main() {
    float heightFactor = grassBlade[gl_InstanceIndex].data.x;
    float stiffness = grassBlade[gl_InstanceIndex].data.y;
    float bladeInitialRotation= grassBlade[gl_InstanceIndex].position.w;

    float windDir= wind.x;
    float windPowerFactor = wind.y;
    float windFrequ = wind.z;
    float windAmpl = wind.w;

    vec3 bladePos = vec3(inPosition.x, inPosition.y , inPosition.z);
    vec3 bladeOffset = vec3(grassBlade[gl_InstanceIndex].position.x, grassBlade[gl_InstanceIndex].position.y, grassBlade[gl_InstanceIndex].position.z + bladePos.z +(bladePos.z* heightFactor));
    
    float baseWindPower = pNoise(vec2(grassBlade[gl_InstanceIndex].position.x, grassBlade[gl_InstanceIndex].position.y) * windAmpl - vec2(sin(windDir),cos(windDir)) * ubo.time * windFrequ,1); // [0,1]
    float windPower = baseWindPower * stiffness;


    float curvFactor = (windPowerFactor + windPower) * bladePos.z;
    mat3 curvature = mat3(1 , 0,             0,
                          0, cos(curvFactor), -sin(curvFactor),
                          0, sin(curvFactor), cos(curvFactor)
                    );

    mat3 curvatureNormal = mat3(cos(curvFactor),0 , -sin(curvFactor),
                                0,              1 , 0,
                                sin(curvFactor),0 , cos(curvFactor)
                    );

    mat3 rotation = mat3( cos(bladeInitialRotation), -sin(bladeInitialRotation), 0,
                          sin(bladeInitialRotation),  cos(bladeInitialRotation), 0,
                          0,           0,          1
                    );

    vec3 bladePosWindEffect = rotation * curvature * bladePos;
    vec3 bladeNormWindEffect =  rotation * curvatureNormal * inNormal;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(bladePosWindEffect + bladeOffset,1.0f);

    fragColor = inColor;    
    fragNormal = bladeNormWindEffect;
    fragTexCoord = inTexCoord;

    vec4 vertPos4 =  ubo.model * vec4(bladePosWindEffect + bladeOffset, 1.0);
    fragVertPos = vec3(vertPos4);
}