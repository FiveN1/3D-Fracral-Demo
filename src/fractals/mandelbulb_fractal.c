#include"common.h"

static float de(vec3_t p) {
    int iterations = fractal_state.fs_params.frac_iterations;
    p.z += 3.0f;
    vec3_t w = p;
    float m = vec3_dot(w, w);
    vec3_t w_abs = vec3_abs(w);
    vec4_t trap = vec4(w_abs.x, w_abs.y, w_abs.z, m);
    float dz = 1.0;
    for (int i = 0; i < iterations; i++) {
        float m2 = m * m;
        float m4 = m2 * m2;
        dz = 8.0 * vecmath_sqrt(m4 * m2 * m) * dz + 1.0;
        float x = w.x; float x2 = x * x; float x4 = x2 * x2;
        float y = w.y; float y2 = y * y; float y4 = y2 * y2;
        float z = w.z; float z2 = z * z; float z4 = z2 * z2;
        float k3 = x2 + z2;
        float k2 = 1.0f / vecmath_sqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
        float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
        float k4 = x2 - y2 + z2;
        w.x = p.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
        w.y = p.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
        w.z = p.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2;
        w_abs = vec3_abs(w);
        trap = vec4_min(trap, vec4(w_abs.x, w_abs.y, w_abs.z, m));
        m = vec3_dot(w, w);
        if (m > 256.0) break;
    }
    return 0.25 * vecmath_log(m) * vecmath_sqrt(m) / dz;
}

fractal_t mandelbulb_fractal = {
    .name = "mandelbulb fractal",
    .dist_func = &de,
    .shader_desc_cb = &mandelbulb_fractal_shd_shader_desc
};