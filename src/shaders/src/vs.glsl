@vs vs
layout(binding = 0) uniform vs_params{
    float aspect;
    vec3 eye_position;
    vec3 eye_direction;
};

in vec4 position;
out vec2 pos;
out vec3 eye;
out vec3 up;
out vec3 right;
out vec3 fwd;

void lookat(vec3 eye, vec3 center, vec3 up, out vec3 out_fwd, out vec3 out_right, out vec3 out_up) {
    out_fwd = normalize(eye_direction);
    out_right = normalize(cross(out_fwd, up));
    out_up = cross(out_right, out_fwd);
}

void main() {
    gl_Position = position;
    pos.x = position.x * aspect;
    pos.y = position.y;
    const vec3 center = vec3(0.0, 0.0, 0.0);
    const vec3 up_vec = vec3(0.0, 1.0, 0.0);
    eye = eye_position;
    lookat(eye, center, up_vec, fwd, right, up);
}
@end