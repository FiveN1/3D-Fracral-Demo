#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    #define M_PI 3.1415
    #define D (vec3_dot(vec3_sin(Q), vec3_cos(vec3(Q.y, Q.z, Q.x))) + 1.3)
    vec3_t Q;
    float i, d = 1.0f;
    Q = p; d = D; Q.x += M_PI; d = vecmath_min(d, D);
    Q.y += M_PI;
    d = vecmath_min(d, D);
    Q = vec3_mulf(Q, 30.0f);
    d = vecmath_max(vecmath_abs(d), (vecmath_abs(D - 1.3f) - 0.5f) / 30.0f);
    return d * 0.6;
}

fractal_t tube_fractal = {
    .name = "tube fractal",
    .dist_func = &de,
    .shader_desc_cb = &tube_fractal_shd_shader_desc
};