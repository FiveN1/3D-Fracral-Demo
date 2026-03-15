
// Source:
// https://jbaker.graphics/writings/DEC.html

// --- web fractal
@block web_fractal
float de(vec3 p, int iterations) {
    p.y += -2.0;
    p.z += -1.0;
    p = p.xzy;
    vec3 cSize = vec3(1.0, 1.0, 1.3);
    float scale = 1.0;
    for (int i = 0; i < iterations; i++) { // 12
        p = 2.0 * clamp(p, -cSize, cSize) - p;
        float r2 = dot(p, p + sin(p.z * 0.3));
        float k = max((2.0) / (r2), 0.027);
        p *= k;  scale *= k;
    }
    float l = length(p.xy);
    float rxy = l - 4.0;
    float n = l * p.z;
    rxy = max(rxy, -(n) / 4.0);
    return (rxy) / abs(scale);
}
@end

// --- stronghold fractal
@block stronghold_fractal
float de(vec3 p, int iterations) {
    float s = 2.0;
    float e = 0.0;
    for (int j = 0; ++j < iterations;) // iterations = 7
        p.xz = abs(p.xz) - 2.3,
        p.z > p.x ? p = p.zyx : p,
        p.z = 1.5 - abs(p.z - 1.3 + sin(p.z) * 0.2),
        p.y > p.x ? p = p.yxz : p,
        p.x = 3.0 - abs(p.x - 5.0 + sin(p.x * 3.0) * 0.2),
        p.y > p.x ? p = p.yxz : p,
        p.y = 0.9 - abs(p.y - 0.4),
        e = 12.0 * clamp(0.3 / min(dot(p, p), 1.0), 0.0, 1.0) +
        2.0 * clamp(0.1 / min(dot(p, p), 1.0), 0.0, 1.0),
        p = p * e - vec3(7, 1, 1),
        s *= e;
    return length(p) / s;
}
@end

// --- mandelbulb fractal
@block mandelbulb_fractal
float de(vec3 p, int iterations) {
    p.z += 3.0f;
    vec3 w = p;
    float m = dot(w, w);
    float dz = 1.0;
    for (int i = 0; i < iterations; i++) {
        float m2 = m * m;
        float m4 = m2 * m2;
        dz = 8.0 * sqrt(m4 * m2 * m) * dz + 1.0;
        float x = w.x; float x2 = x * x; float x4 = x2 * x2;
        float y = w.y; float y2 = y * y; float y4 = y2 * y2;
        float z = w.z; float z2 = z * z; float z4 = z2 * z2;
        float k3 = x2 + z2;
        float k2 = inversesqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
        float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
        float k4 = x2 - y2 + z2;
        w.x = p.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
        w.y = p.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
        w.z = p.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2;
        m = dot(w, w);
        if (m > 256.0) break;
    }
    return 0.25 * log(m) * sqrt(m) / dz;
}
@end

// warping cubes fractal
@block warping_cubes_fractal
float de(vec3 pos, int iterations) {
#define SCALE 2.8
#define MINRAD2 .25
#define scale (vec4(SCALE, SCALE, SCALE, abs(SCALE)) / minRad2)
    pos.z += 3.0;
    float minRad2 = clamp(MINRAD2, 1.0e-9, 1.0);
    float absScalem1 = abs(SCALE - 1.0);
    float AbsScale = pow(abs(SCALE), float(1 - 10));
    vec4 p = vec4(pos, 1);
    vec4 p0 = p;
    for (int i = 0; i < iterations; i++) // iterations = 9
    {
        p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;
        float r2 = dot(p.xyz, p.xyz);
        p *= clamp(max(minRad2 / r2, minRad2), 0.0, 1.0);
        p = p * scale + p0;
    }
    return ((length(p.xyz) - absScalem1) / p.w - AbsScale);
#undef SCALE
#undef MINRAD2
#undef scale
}
@end

// tube fractal
@block tube_fractal
#define M_PI 3.1415
#define D (dot(sin(Q),cos(Q.yzx))+1.3)
float de(vec3 p, int iterations) {
    vec3 Q;
    float i, d = 1.;
    Q = p, d = D, Q.x += M_PI, d = min(d, D);
    Q.y += M_PI;
    d = min(d, D);
    Q *= 30.;
    d = max(abs(d), (abs(D - 1.3) - .5) / 30.);
    return d * .6;
}
@end

// monolith fractal
@block monolith_fractal
float de(vec3 p, int iterations) {
    p.z += 2.5;
    float s = 3.;
    float e = 0.;
    for (int j = 0; j++ < iterations;)
        s *= e = 3.8 / clamp(dot(p, p), 0., 2.),
        p = abs(p) * e - vec3(1, 15, 1);
    return length(cross(p, vec3(1, 1, -1) * .577)) / s;
}
@end

// tdhooper variant 1 fractal
@block tdhooper1_fractal

// tdhooper variant 1 - spherical inversion
vec2 wrap(vec2 x, vec2 a, vec2 s) {
    x -= s;
    return (x - a * floor(x / a)) + s;
}

void TransA(inout vec3 z, inout float DF, float a, float b) {
    float iR = 1. / dot(z, z);
    z *= -iR;
    z.x = -b - z.x;
    z.y = a + z.y;
    DF *= iR; // max( 1.0, iR );
}

float de(vec3 z, int iterations) {
    z.y += 1.0f;
    z.z += 2.0f;
    vec3 InvCenter = vec3(0.0, 1.0, 1.0);
    float rad = 0.8;
    float KleinR = 1.5 + 0.39;
    float KleinI = (0.55 * 2.0 - 1.0);
    vec2 box_size = vec2(-0.40445, 0.34) * 2.0;
    vec3 lz = z + vec3(1.0), llz = z + vec3(-1.0);
    float d = 0.0; float d2 = 0.0;
    z = z - InvCenter;
    d = length(z);
    d2 = d * d;
    z = (rad * rad / d2) * z + InvCenter;
    float DE = 1e12;
    float DF = 1.0;
    float a = KleinR;
    float b = KleinI;
    float f = sign(b) * 0.45;
    for (int i = 0; i < iterations; i++) {
        z.x += b / a * z.y;
        z.xz = wrap(z.xz, box_size * 2.0, -box_size);
        z.x -= b / a * z.y;
        if (z.y >= a * 0.5 + f * (2.0 * a - 1.95) / 4.0 * sign(z.x + b * 0.5) *
            (1.0 - exp(-(7.2 - (1.95 - a) * 15.0) * abs(z.x + b * 0.5)))) {
            z = vec3(-b, a, 0.0) - z;
        } //If above the separation line, rotate by 180° about (-b/2, a/2)
        TransA(z, DF, a, b); //Apply transformation a
        if (dot(z - llz, z - llz) < 1e-5) {
            break;
        } //If the iterated points enters a 2-cycle, bail out
        llz = lz; lz = z; //Store previous iterates
    }
    float y = min(z.y, a - z.y);
    DE = min(DE, min(y, 0.3) / max(DF, 2.0));
    DE = DE * d2 / (rad + d * DE);
    return DE;
}
@end

@block yonatan_fractal
float de(vec3 p, int iterations) {
    float e, s, t = 0.0; // time adjust term
    vec3 q = p;
    p.z += 7.0;
    s = length(p);
    p = vec3(log(s), atan(p.y, p.x), sin(t / 4.0 + p.z / s));
    s = 1.0;
    for (int j = 0; j++ < iterations;) {
        e = 3.1415 / min(dot(p, p), 0.8);
        s *= e;
        p = abs(p) * e - 3.0;
        p.y -= round(p.y);
    }
    e = length(p) / s;
    return e;
}
@end