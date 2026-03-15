#include"fractals/common.h"
#include"camera.h"

/*
    3D fractal demo by Tomáš Verheyen aka. FiveN.
    This demo is written in C99.
    
    Resources:
        Signed distance field functions, provided by Jon Baker:
        https://jbaker.graphics/writings/DEC.html

        Sokol cross-platform libraries, by floooh:
        https://github.com/floooh/sokol
*/

extern fractal_t web_fractal;
extern fractal_t stronghold_fractal;
extern fractal_t mandelbulb_fractal;
extern fractal_t warping_cubes_fractal;
extern fractal_t tube_fractal;
extern fractal_t monolith_fractal;
extern fractal_t tdhooper1_fractal;
extern fractal_t yonatan_fractal;

static fractal_t* fractals[] = {
    &mandelbulb_fractal,
    &stronghold_fractal,
    &web_fractal,
    &warping_cubes_fractal,
    &tube_fractal,
    &monolith_fractal,
    &tdhooper1_fractal,
    &yonatan_fractal
};

static struct app_data {
    sg_pass_action pass_action;
    camera_t camera;
    bool show_window;
    int fractal_type;
} app_data;

#include"ui.h"

void init() {
    sg_setup(&(sg_desc){ .environment = sglue_environment() });
    stm_setup(); srand(stm_now()); // not great
    simgui_setup(&(simgui_desc_t) { 0 });
    ImGuiIO* ig_io = igGetIO_Nil();
    ig_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    app_data.pass_action = (sg_pass_action){ .colors[0] = {.load_action = SG_LOADACTION_DONTCARE } };
    app_data.show_window = true;
    app_data.fractal_type = rand() % (sizeof(fractals) / sizeof(fractal_t*));
    app_data.camera = (camera_t){
        .pos = (vec3_t){ 0.0f, 0.0f, 0.0f },
        .dir = (vec3_t){ 0.0f, 0.0f, -1.0f },
        .up = (vec3_t){ 0.0f, 1.0f, 0.0f },
        .speed = 4.0f,
        .variable_speed = true
    };

    fractal_state_init();
    fractal_state_set_fractal(fractals[app_data.fractal_type]);
}

void delete() {
    simgui_shutdown();
    sg_shutdown();
}

void frame() {
    // update
    camera_process(&app_data.camera);
    app_data.camera.dist = fractals[app_data.fractal_type]->dist_func(app_data.camera.pos);
    fractal_state.vs_params.aspect = (float)sapp_width() / (float)sapp_height();
    fractal_state.vs_params.eye_position = app_data.camera.pos;
    fractal_state.vs_params.eye_direction = app_data.camera.dir;
    // render
    sg_begin_pass(&(sg_pass){ .action = app_data.pass_action, .swapchain = sglue_swapchain() });
    fractal_state_draw();
    draw_imgui();
    sg_end_pass();
    sg_commit();
}

void on_event(const sapp_event* event) {
    if (simgui_handle_event(event)) return;
    camera_on_event(&app_data.camera, event);
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN) if (event->key_code == SAPP_KEYCODE_H) app_data.show_window = !app_data.show_window; // toggle window
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc) {
        .width = 1920,
        .height = 1080,
        .window_title = "3D Fractal Demo",
        .init_cb = &init,
        .cleanup_cb = &delete,
        .frame_cb = &frame,
        .event_cb = &on_event,
        .high_dpi = true    
    };
}
