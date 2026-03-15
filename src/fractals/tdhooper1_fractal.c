#include"common.h"

vec2_t wrap(vec2_t x, vec2_t a, vec2_t s) {
    x = vec2_sub(x, s);
    return (vec2_add(vec2_sub(x, vec2_mul(a, vec2_floor(vec2_div(x, a)))), s));
}

void TransA(vec3_t* z, float* DF, float a, float b) {
    float iR = 1. / vec3_dot(*z, *z);
    *z = vec3_mulf(*z, -iR);
    z->x = -b - z->x;
    z->y = a + z->y;
    *DF *= iR; // max( 1.0, iR );
}

static float de(vec3_t z) {
    int iterations = fractal_state.fs_params.frac_iterations;
    z.y += 1.0f;
    z.z += 2.0f;
    vec3_t InvCenter = vec3(0.0, 1.0, 1.0);
    float rad = 0.8;
    float KleinR = 1.5 + 0.39;
    float KleinI = (0.55 * 2.0 - 1.0);
    vec2_t box_size = vec2_mulf(vec2(-0.40445, 0.34), 2.0);
    vec3_t lz = vec3_add(z, vec3f(1.0)), llz = vec3_add(z, vec3f(-1.0));
    float d = 0.0; float d2 = 0.0;
    z = vec3_sub(z, InvCenter);
    d = vec3_length(z);
    d2 = d * d;
    z = vec3_add(vec3_mulf(z, (rad * rad / d2)), InvCenter);
    float DE = 1e12;
    float DF = 1.0;
    float a = KleinR;
    float b = KleinI;
    float f = vecmath_sign(b) * 0.45;
    for (int i = 0; i < iterations; i++) {
        z.x += b / a * z.y;
        vec2_t xz = wrap(vec2(z.x, z.z), vec2_mulf(box_size, 2.0), vec2_neg(box_size));
        z.x = xz.x;
        z.z = xz.y;
        z.x -= b / a * z.y;
        if (z.y >= a * 0.5 + f * (2.0 * a - 1.95) / 4.0 * vecmath_sign(z.x + b * 0.5) *
            (1.0 - vecmath_exp(-(7.2 - (1.95 - a) * 15.0) * vecmath_abs(z.x + b * 0.5)))) {
            z = vec3_sub(vec3(-b, a, 0.0), z);
        } //If above the separation line, rotate by 180� about (-b/2, a/2)
        TransA(&z, &DF, a, b); //Apply transformation a
        if (vec3_dot(vec3_sub(z, llz), vec3_sub(z, llz)) < 1e-5) {
            break;
        } //If the iterated points enters a 2-cycle, bail out
        llz = lz; lz = z; //Store previous iterates
    }
    float y = vecmath_min(z.y, a - z.y);
    DE = vecmath_min(DE, vecmath_min(y, 0.3) / vecmath_max(DF, 2.0));
    DE = DE * d2 / (rad + d * DE);
    return DE;
}

fractal_t tdhooper1_fractal= {
    .name = "tdhooper 1 fractal",
    .dist_func = &de,
    .shader_desc_cb = &tdhooper1_fractal_shd_shader_desc
};