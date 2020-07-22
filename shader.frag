#version 400

#define MAX_ITER 1000.
#define HIGH_QUALITY

#ifdef HIGH_QUALITY
    #define vec2 dvec2
    #define vec3 dvec3
    #define float double
#endif

uniform float time;
uniform ivec2 resolution;

uniform vec2 mandelbrotCenter;
uniform float mandelbrotSize;

out vec4 fragColor;

vec2 cadd(in vec2 a, in vec2 b) { return a + b; }
vec2 cmul(in vec2 a, in vec2 b) { return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x); }
float cabs(in vec2 a) { return length(a); }

vec3 getColor(in float v) {
    const float c = 2.;
    float i = 1. / c;
    for (; i > exp(-100.) && v > 1. - i; i /= c);
    i *= c;
    float x = c*(v - 1 + i)/i;

    x = 4.*x*(1.-x);
    x = x*x*(3.-2.*x);

    return mix(vec3(1., .647, .0), vec3(0.), x);
}

float mandelbrot(in vec2 uv) {
    vec2 z = vec2(0.);
    vec2 c = uv;

    for (float i = 0.; i < MAX_ITER; ++i) {
        if (cabs(z) > 2.) return i / MAX_ITER;

        z = cadd(cmul(z, z), c);
    }
    return 1.;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5*vec2(resolution)) / float(min(resolution.x, resolution.y));
    uv = (uv * mandelbrotSize) + mandelbrotCenter;

    float m = mandelbrot(uv);
    vec3 col = getColor(m);
    col = sqrt(col);

    fragColor = vec4(col, 1.);
}
