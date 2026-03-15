#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    float e, s, t = 0.0; // time adjust term
    vec3_t q = p;
    p.z += 7.0;
    s = vec3_length(p);
    p = vec3(vecmath_log(s), vecmath_atan2(p.y, p.x), vecmath_sin(t / 4.0f + p.z / s));
    s = 1.0;
    for (int j = 0; j++ < iterations;) {
        e = 3.1415f / vecmath_min(vec3_dot(p, p), 0.8f);
        s *= e;
        p = vec3_subf(vec3_mulf(vec3_abs(p), e), 3.0);
        p.y -= vecmath_round(p.y);
    }
    e = vec3_length(p) / s;
    return e;
}

fractal_t yonatan_fractal = {
    .name = "yonatan fractal",
    .dist_func = &de,
    .shader_desc_cb = &yonatan_fractal_shd_shader_desc
};