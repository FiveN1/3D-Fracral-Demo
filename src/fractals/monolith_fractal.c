#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    p.z += 2.5;
    float s = 3.0f;
    float e = 0.0f;
    for (int j = 0; j++ < iterations;)
        s *= e = 3.8 / vecmath_clamp(vec3_dot(p, p), 0.0f, 2.0f),
        p = vec3_sub(vec3_mulf(vec3_abs(p), e), vec3(1.0f, 15.0f, 1.0f));
    return vec3_length(vec3_cross(p, vec3_mulf(vec3(1.0f, 1.0f, -1.0f), 0.577f))) / s;
}

fractal_t monolith_fractal = {
    .name = "monolith fractal",
    .dist_func = &de,
    .shader_desc_cb = &monolith_fractal_shd_shader_desc
};