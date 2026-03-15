#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    p.y += -2.0;
    p.z += -1.0;
    p = vec3(p.x, p.z, p.y);
    vec3_t cSize = vec3(1.0f, 1.0f, 1.3f);
    float scale = 1.0;
    for (int i = 0; i < iterations; i++) {
        p = vec3_sub(vec3_mulf(vec3_clamp(p, vec3_neg(cSize), cSize), 2.0f), p);
        float r2 = vec3_dot(p, vec3_addf(p, vecmath_sin(p.z * 0.3f)));
        float k = vecmath_max((2.0f) / (r2), 0.027f);
        p = vec3_mulf(p, k); scale *= k;
    }
    float l = vec2_length(vec2(p.x, p.y));
    float rxy = l - 4.0f;
    float n = l * p.z;
    rxy = vecmath_max(rxy, -(n) / 4.0f);
    return (rxy) / vecmath_abs(scale);
}

fractal_t web_fractal = {
    .name = "web fractal",
    .dist_func = &de,
    .shader_desc_cb = &web_fractal_shd_shader_desc
};