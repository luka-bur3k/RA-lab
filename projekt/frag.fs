#version 330 core

#define MAX_ITERATIONS_DE 16
#define MAX_DISTANCE_DE 2.0
#define MAX_ITERATIONS_RAY 64
#define STEP_RAY 0.5
#define MIN_STEP_RAY 0.1
#define POWER 8.0

#define ASPECT_RATIO 320.0/240.0
#define RESOLUTION vec2(320.0, 240.0)

out vec4 color;


uniform float time;
uniform vec3 camPosition;
uniform vec4 camRotationQuaternion;
uniform vec4 camRotationQuaternionConjugate;
//uniform mat4 projectionMatrix;
// uniform mat4 viewMatrix;

const float PI = 3.1415926535897;

vec2 mandelbulbDistanceEstimator(vec3 z) {

    // constant 
    vec3 w = z;

    float dr = 1.0;
    float r = 0.0;

    float Power = 4.0 + 3.0*(1 + sin(time/10.0));

    int steps = 0;
    for(int i = 0; i < MAX_ITERATIONS_DE; ++i)
    {
        ++steps;
        r = length(z);

        // break if we assume point is diverging
        if(r > MAX_DISTANCE_DE)
        {
            break;
        }

        // convert to polar coordinates 
        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        dr = pow(r, Power - 1.0) * Power * dr + 1.0;

        // scale and rotate the point ?
        r = pow(r, Power);
        theta *= Power;
        phi *= Power;
        
        // convert back to cartesian coordinates
        // z_n+1 = z_n^POWER + w
        z = r * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += w;
    }

    return vec2(0.5*log(r)*r/dr, steps);
}

vec2 mendelbulbRayMarching(vec3 z, vec3 rayDirection) {
    
    float t = 0.0;
    float c = 0.0;

    for(int i = 0; i < MAX_ITERATIONS_RAY; ++i)
    {
        vec3 path = z + rayDirection * t;
        vec2 dist = mandelbulbDistanceEstimator(path);

        t += STEP_RAY * dist.x;
        c += dist.y;
    }

    return vec2(t, c / 2048.0 - t/100.0);
}

void main() {  


    // convert uv from domain [(0,0),(640,480)] to [(-1,-1),(1,1)]
    vec2 uv = gl_FragCoord.xy / RESOLUTION;
    uv = uv * 2.0 - 1.0;
    // adjusting aspect ratio
    // uv.x *= ASPECT_RATIO; 

    vec3 ray = normalize(vec3(uv, -1.0));
    ray = ray + cross(2.0 * camRotationQuaternion.xyz, cross(camRotationQuaternion.xyz, ray) + camRotationQuaternion.w * ray);

    // convert from clip space to worldspace 
    // vec4 position =  inverse(projectionMatrix * viewMatrix) * vec4(uv, 0.0, 1.0);
    vec2 result = mendelbulbRayMarching(camPosition, ray);

    color = vec4(  result.y*(1 + cos(time + result.x)), result.y*(1 + sin((time + result.x)/2.0)) ,  result.y*(1 + cos(time/1.25)), 1.0);
}


// References :
// http://www.zemris.fer.hr/predmeti/ra/LabRadovi/dokumentacija/2022AnteMili%C4%8Devi%C4%87.pdf
// https://www.shadertoy.com/view/wdjGWR
// https://arxiv.org/pdf/2102.01747.pdf
// https://en.wikipedia.org/wiki/Mandelbulb