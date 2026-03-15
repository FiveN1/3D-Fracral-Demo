#include"common.h"

static float de(vec3_t pos) {
    int iterations = fractal_state.fs_params.frac_iterations;
#define SCALE 2.8f
#define MINRAD2 0.25f
#define scale (vec4_divf(vec4(SCALE, SCALE, SCALE, vecmath_abs(SCALE)), minRad2))
    pos.z += 3.0;
    float minRad2 = vecmath_clamp(MINRAD2, (float)1.0e-9, 1.0f);
    float absScalem1 = vecmath_abs(SCALE - 1.0);
    float AbsScale = vecmath_pow(vecmath_abs(SCALE), (float)(1.0f - 10.0f));
    vec4_t p = vec4(pos.x, pos.y, pos.z, 1.0f);
    vec4_t p0 = p;
    for (int i = 0; i < iterations; i++) {
        vec3_clamp(vec3(p.x, p.y, p.z), vec3f(-1.0f), vec3f(1.0f));
        vec3_t clamped = vec3_clamp(vec3(p.x, p.y, p.z), vec3f(-1.0f), vec3f(1.0f));
        vec3_t final = vec3_sub(vec3_mulf(clamped, 2.0f), vec3(p.x, p.y, p.z));
        p = vec4v3f(final, p.w);
        float r2 = vec3_dot(vec3(p.x, p.y, p.z), vec3(p.x, p.y, p.z));
        p = vec4_mulf(p, vecmath_clamp(vecmath_max(minRad2 / r2, minRad2), 0.0, 1.0));
        p = vec4_add(vec4_mul(p, scale), p0);
    }
    return ((vec3_length(vec3(p.x, p.y, p.z)) - absScalem1) / p.w - AbsScale);
#undef SCALE
#undef MINRAD2
#undef scale
}

fractal_t warping_cubes_fractal = {
    .name = "warping cubes fractal",
    .dist_func = &de,
    .shader_desc_cb = &warping_cubes_fractal_shd_shader_desc
};