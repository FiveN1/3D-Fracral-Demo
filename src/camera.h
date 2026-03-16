
typedef struct camera_t {
    vec3_t pos;
    vec3_t dir;
    vec3_t up;
    float speed;
    float sensitivity;
    float dist;                 // distance from fractal
    bool variable_speed;
    
    struct {                    // not very pretty to look at
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

void camera_process(camera_t* camera) {
    float camera_speed = camera->speed * (float)sapp_frame_duration();
    if (camera->variable_speed) camera_speed *= camera->dist;
    if (camera->key_states.shift_pressed) camera_speed *= 2.0f;
    vec3_t camera_right = vec3_normalize(vec3_cross(camera->dir, camera->up));
    if (camera->key_states.w_pressed) camera->pos = vec3_add(camera->pos, vec3_mulf(camera->dir, camera_speed));
    if (camera->key_states.s_pressed) camera->pos = vec3_sub(camera->pos, vec3_mulf(camera->dir, camera_speed));
    if (camera->key_states.a_pressed) camera->pos = vec3_sub(camera->pos, vec3_mulf(camera_right, camera_speed));
    if (camera->key_states.d_pressed) camera->pos = vec3_add(camera->pos, vec3_mulf(camera_right, camera_speed));
    if (camera->key_states.space_pressed) camera->pos = vec3_add(camera->pos, vec3_mulf(camera->up, camera_speed));
    if (camera->key_states.ctrl_pressed) camera->pos = vec3_sub(camera->pos, vec3_mulf(camera->up, camera_speed));
}

void camera_on_event(camera_t* camera, const sapp_event* event) {
    // camera movement
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (event->key_code == SAPP_KEYCODE_W) camera->key_states.w_pressed = true;
        if (event->key_code == SAPP_KEYCODE_S) camera->key_states.s_pressed = true;
        if (event->key_code == SAPP_KEYCODE_A) camera->key_states.a_pressed = true;
        if (event->key_code == SAPP_KEYCODE_D) camera->key_states.d_pressed = true;
        if (event->key_code == SAPP_KEYCODE_LEFT_SHIFT) camera->key_states.shift_pressed = true;
        if (event->key_code == SAPP_KEYCODE_SPACE) camera->key_states.space_pressed = true;
        if (event->key_code == SAPP_KEYCODE_LEFT_CONTROL) camera->key_states.ctrl_pressed = true;
    }
    if (event->type == SAPP_EVENTTYPE_KEY_UP) {
        if (event->key_code == SAPP_KEYCODE_W) camera->key_states.w_pressed = false;
        if (event->key_code == SAPP_KEYCODE_S) camera->key_states.s_pressed = false;
        if (event->key_code == SAPP_KEYCODE_A) camera->key_states.a_pressed = false;
        if (event->key_code == SAPP_KEYCODE_D) camera->key_states.d_pressed = false;
        if (event->key_code == SAPP_KEYCODE_LEFT_SHIFT) camera->key_states.shift_pressed = false;
        if (event->key_code == SAPP_KEYCODE_SPACE) camera->key_states.space_pressed = false;
        if (event->key_code == SAPP_KEYCODE_LEFT_CONTROL) camera->key_states.ctrl_pressed = false;
    }
    // toggle cammera
    if (event->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            camera->key_states.can_look = !camera->key_states.can_look;
            sapp_lock_mouse(camera->key_states.can_look);
        }
    }
    // move camera
    if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE && camera->key_states.can_look) {
        float sensitivity = camera->sensitivity * (float)sapp_frame_duration();
        vec2_t camera_rotation = (vec2_t){
            .x = sensitivity * event->mouse_dy,
            .y = sensitivity * event->mouse_dx
        };
        vec4_t x_rotation = quat_rotation_axis(vec3_normalize(vec3_cross(camera->dir, camera->up)), camera_rotation.y); // right vector
        vec4_t y_rotation = quat_rotation_axis(camera->up, -camera_rotation.x);
        vec3_t rotation = (vec3_t){
            .x = x_rotation.x + y_rotation.x,
            .y = x_rotation.y + y_rotation.y,
            .z = x_rotation.z + y_rotation.z
        };
        camera->dir = vec3_add(camera->dir, rotation);
    }
}