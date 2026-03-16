@block fs_block

layout(binding = 1) uniform fs_params{
    // sphere tracing variables
    float min_dist;
    float max_dist;
    float focal_length;
    int frac_iterations;
    int max_march_steps;
    // lighting variables
    vec3 ambient_color;
    vec3 light_color;
    vec3 light_direction;
    vec3 spec_color;
    float ambient_strength;
    float spec_strength;
    float spec_power;
};

in vec2 pos;
in vec3 eye;
in vec3 up;
in vec3 right;
in vec3 fwd;
out vec4 frag_color;

// distance function of scene
// the de function is included from fractal_funcs.glsl in the shaders.glsl 
float de_scene(vec3 p) {
    return de(p, frac_iterations);
}

// surface normal estimation
vec3 surface_normal(vec3 p, float dp, float t) {
    const vec2 d = vec2(min_dist * t, 0);
    float x = de_scene(p + d.xyy) - dp;
    float y = de_scene(p + d.yxy) - dp;
    float z = de_scene(p + d.yyx) - dp;
    return normalize(vec3(x, y, z));
}

// get lighting of fragment
vec3 get_light(vec3 p, vec3 obj_color, vec3 norm, vec3 ray_direction) {
    vec3 L = normalize(light_direction);
    vec3 R = reflect(light_direction, norm);
    vec3 ambient = ambient_strength * ambient_color;
    vec3 diffuse = clamp(dot(L, norm), 0.0, 1.0) * light_color;
    vec3 specular = spec_color * spec_strength * pow(clamp(dot(R, ray_direction), 0.0, 1.0), spec_power);
    return (ambient + diffuse) * obj_color + specular;
}

void main() {
    vec3 ray_origin = eye;
    vec3 ray_direction = normalize(fwd * focal_length + (right * pos.x + up * pos.y));
    float ambient_strength = 0.2f;
    vec3 object_color = vec3(1.0);
    vec4 color = vec4(ambient_color, 1.0);
    float t = 0.0;
    for (int i = 0; i < max_march_steps; i++) {
        vec3 p = ray_origin + ray_direction * t;
        float d = de_scene(p);
        if (d < min_dist * t) {
            vec3 norm = surface_normal(p, d, t);
            float ambient_occlusion = 1.0f - float(i) / float(max_march_steps);
            float fog = 1.0 - length(eye - p) / max_dist;
            color.xyz = get_light(p, object_color, norm, ray_direction) * ambient_occlusion * fog + ambient_color * (1.0f - fog);
            break;
        }
        if (t > max_dist) break;
        t += d;
    }
    frag_color = color;
}
@end