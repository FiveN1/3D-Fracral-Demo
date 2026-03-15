#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    float s = 2.0;
    float e = 0.0;
    for (int j = 0; ++j < iterations;) {// iterations = 7
        vec2_t xz = vec2_subf(vec2_abs(vec2(p.x, p.z)), 2.3f);
        p.x = xz.x;
        p.z = xz.y;
        p.z > p.x ? p = vec3(p.z, p.y, p.x) : p;
        p.z = 1.5f - vecmath_abs(p.z - 1.3f + vecmath_sin(p.z) * 0.2f);
        p.y > p.x ? p = vec3(p.y, p.x, p.z) : p;
        p.x = 3.0f - vecmath_abs(p.x - 5.0f + vecmath_sin(p.x * 3.0f) * 0.2f);
        p.y > p.x ? p = vec3(p.y, p.x, p.z) : p;
        p.y = 0.9f - vecmath_abs(p.y - 0.4f);
        e = 12.0f * vecmath_clamp(0.3f / vecmath_min(vec3_dot(p, p), 1.0f), 0.0f, 1.0f) +
            2.0f * vecmath_clamp(0.1f / vecmath_min(vec3_dot(p, p), 1.0f), 0.0f, 1.0f);
        p = vec3_sub(vec3_mulf(p, e), vec3(7.0f, 1.0f, 1.0f));
        s *= e;
    }
    return vec3_length(p) / s;
}

fractal_t stronghold_fractal = {
    .name = "stronghold fractal",
    .dist_func = &de,
    .shader_desc_cb = &stronghold_fractal_shd_shader_desc
};