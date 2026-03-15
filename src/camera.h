
typedef struct camera_t {
    vec3_t pos;
    vec3_t dir;
    vec3_t up;
    float speed;
    float dist;                 // distance from fractal
    bool variable_speed;

    struct {
        bool w_pressed;
        bool s_pressed;
        bool a_pressed;
        bool d_pressed;
        bool can_look;
        bool shift_pressed;
        bool space_pressed;
        bool ctrl_pressed;
    } key_states;
} camera_t;

void camera_process(camera_t* eye) {
    float eye_speed = eye->speed * (float)sapp_frame_duration();
    if (eye->variable_speed) eye_speed *= eye->dist;
    if (eye->key_states.shift_pressed) eye_speed *= 2.0f;
    vec3_t eye_right = vec3_normalize(vec3_cross(eye->dir, eye->up));
    if (eye->key_states.w_pressed) eye->pos = vec3_add(eye->pos, vec3_mulf(eye->dir, eye_speed));
    if (eye->key_states.s_pressed) eye->pos = vec3_sub(eye->pos, vec3_mulf(eye->dir, eye_speed));
    if (eye->key_states.a_pressed) eye->pos = vec3_sub(eye->pos, vec3_mulf(eye_right, eye_speed));
    if (eye->key_states.d_pressed) eye->pos = vec3_add(eye->pos, vec3_mulf(eye_right, eye_speed));
    if (eye->key_states.space_pressed) eye->pos = vec3_add(eye->pos, vec3_mulf(eye->up, eye_speed));
    if (eye->key_states.ctrl_pressed) eye->pos = vec3_sub(eye->pos, vec3_mulf(eye->up, eye_speed));
}

void camera_on_event(camera_t* eye, const sapp_event* event) {
    // camera movement
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (event->key_code == SAPP_KEYCODE_W) eye->key_states.w_pressed = true;
        if (event->key_code == SAPP_KEYCODE_S) eye->key_states.s_pressed = true;
        if (event->key_code == SAPP_KEYCODE_A) eye->key_states.a_pressed = true;
        if (event->key_code == SAPP_KEYCODE_D) eye->key_states.d_pressed = true;
        if (event->key_code == SAPP_KEYCODE_LEFT_SHIFT) eye->key_states.shift_pressed = true;
        if (event->key_code == SAPP_KEYCODE_SPACE) eye->key_states.space_pressed = true;
        if (event->key_code == SAPP_KEYCODE_LEFT_CONTROL) eye->key_states.ctrl_pressed = true;
    }
    if (event->type == SAPP_EVENTTYPE_KEY_UP) {
        if (event->key_code == SAPP_KEYCODE_W) eye->key_states.w_pressed = false;
        if (event->key_code == SAPP_KEYCODE_S) eye->key_states.s_pressed = false;
        if (event->key_code == SAPP_KEYCODE_A) eye->key_states.a_pressed = false;
        if (event->key_code == SAPP_KEYCODE_D) eye->key_states.d_pressed = false;
        if (event->key_code == SAPP_KEYCODE_LEFT_SHIFT) eye->key_states.shift_pressed = false;
        if (event->key_code == SAPP_KEYCODE_SPACE) eye->key_states.space_pressed = false;
        if (event->key_code == SAPP_KEYCODE_LEFT_CONTROL) eye->key_states.ctrl_pressed = false;
    }
    // toggle cammera
    if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            eye->key_states.can_look = !eye->key_states.can_look;
            sapp_lock_mouse(eye->key_states.can_look);
        }
    }
    // move camera
    if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE && eye->key_states.can_look) {
        float sensitivity = 0.001f;
        vec2_t eye_rotation = (vec2_t){
            .x = sensitivity * event->mouse_dy,
            .y = sensitivity * event->mouse_dx
        };
        vec4_t x_rotation = quat_rotation_axis(vec3_normalize(vec3_cross(eye->dir, eye->up)), eye_rotation.y); // right vector
        vec4_t y_rotation = quat_rotation_axis(eye->up, -eye_rotation.x);
        vec3_t rotation = (vec3_t){
            .x = x_rotation.x + y_rotation.x,
            .y = x_rotation.y + y_rotation.y,
            .z = x_rotation.z + y_rotation.z
        };
        eye->dir = vec3_add(eye->dir, rotation);
    }
}