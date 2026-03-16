
void draw_imgui() {
    simgui_new_frame(&(simgui_frame_desc_t) {
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
    });

    if (app_data.show_window) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        window_flags |= ImGuiWindowFlags_NoMove;

        static int location = 0;
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = igGetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        igSetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        igSetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;

        igSetNextWindowBgAlpha(0.35f); // Transparent background
        igBegin("3D Fractal Demo", &app_data.show_window, window_flags);
        igText("3D Fractal Demo");
        igSeparator();
        igText("This demo uses the sphere-tracing approach\nfor rendering the SDF of these fractals,\nthus it may be slow on some older hardware!");
        igSeparator();
        igText("ms: %.3f, fps: %.1f", sapp_frame_duration() * 1000.0f, 1.0f / sapp_frame_duration());
        igSeparator();
        igText("CONTROLS");
        igBulletText("use 'WSAD', 'CTRL', 'SHIFT' to move");
        igBulletText("press 'MOUSE LEFT' to toggle camera");
        igBulletText("press 'H' to hide this menu");

        igSeparator();
        const char* items[64];
        int item_count = (int)(sizeof(fractals) / sizeof(fractal_t*));
        for (int i = 0; i < item_count; i++) items[i] = fractals[i]->name;
        if (igCombo_Str_arr("fractal type", &app_data.fractal_type, items, item_count, item_count)) {
            fractal_state_set_fractal(fractals[app_data.fractal_type]);
        }
        igSliderInt("iteration count", &fractal_state.fs_params.frac_iterations, 1, 24, NULL, 0);

        igSeparator();
        igText("SETTINGS");
        if (igCollapsingHeader_TreeNodeFlags("window", 0)) {
            static bool is_fullscreen = false;
            if (igCheckbox("fullscreen", &is_fullscreen)) sapp_toggle_fullscreen();
        }
        if (igCollapsingHeader_TreeNodeFlags("renderer", 0)) {
            igSliderFloat("min dist", &fractal_state.fs_params.min_dist, 0.001f, 0.1f, NULL, 0);
            igSliderFloat("max dist", &fractal_state.fs_params.max_dist, 0.2f, 512.0f, NULL, 0);
            igSliderInt("max raymarch steps", &fractal_state.fs_params.max_march_steps, 1, 512, NULL, 0);
        }
        if (igCollapsingHeader_TreeNodeFlags("camera", 0)) {
            igText("camera pos: %f, %f, %f", app_data.camera.pos.x, app_data.camera.pos.y, app_data.camera.pos.z);
            igText("camera dir: %f, %f, %f", app_data.camera.dir.x, app_data.camera.dir.y, app_data.camera.dir.z);
            if (igButton("reset position", (ImVec2_c) { 0 })) app_data.camera.pos = (vec3_t){ 0.0, 0.0, 0.0 };
            igCheckbox("variable speed", &app_data.camera.variable_speed);
            igSliderFloat("camera speed", &app_data.camera.speed, 0.2f, 64.0f, NULL, 0);
            igSliderFloat("camera sensitivity", &app_data.camera.sensitivity, 0.01f, 1.0f, NULL, 0);
            igSliderFloat("focal length", &fractal_state.fs_params.focal_length, 0.2f, 8.0f, NULL, 0);

        }
        if (igCollapsingHeader_TreeNodeFlags("lighting", 0)) {
            igText("ambient lighting"); {
                float col_arr[] = { fractal_state.fs_params.ambient_color.x, fractal_state.fs_params.ambient_color.y, fractal_state.fs_params.ambient_color.z };
                igColorEdit3("ambient color", col_arr, 0);
                fractal_state.fs_params.ambient_color.x = col_arr[0];
                fractal_state.fs_params.ambient_color.y = col_arr[1];
                fractal_state.fs_params.ambient_color.z = col_arr[2];
                igSliderFloat("ambient strength", &fractal_state.fs_params.ambient_strength, 0.0f, 1.0f, NULL, 0);
            }
            igSeparator();
            igText("directional lighting"); {
                float col_arr[] = { fractal_state.fs_params.light_color.x, fractal_state.fs_params.light_color.y, fractal_state.fs_params.light_color.z };
                igColorEdit3("light color", col_arr, 0);
                fractal_state.fs_params.light_color.x = col_arr[0], fractal_state.fs_params.light_color.y = col_arr[1], fractal_state.fs_params.light_color.z = col_arr[2];
                igSeparator();
                col_arr[0] = fractal_state.fs_params.spec_color.x, col_arr[1] = fractal_state.fs_params.spec_color.y, col_arr[2] = fractal_state.fs_params.spec_color.z;
                igColorEdit3("specular color", col_arr, 0);
                fractal_state.fs_params.spec_color.x = col_arr[0], fractal_state.fs_params.spec_color.y = col_arr[1], fractal_state.fs_params.spec_color.z = col_arr[2];
                igSliderFloat("specular strength", &fractal_state.fs_params.spec_strength, 0.0f, 1.0f, NULL, 0);
                igSliderFloat("specular power", &fractal_state.fs_params.spec_power, 0.0f, 10.0f, NULL, 0);
            }
            igSeparator();
            igSliderFloat("light x", &fractal_state.fs_params.light_direction.x, -1.0f, 1.0f, NULL, 0);
            igSliderFloat("light y", &fractal_state.fs_params.light_direction.y, -1.0f, 1.0f, NULL, 0);
            igSliderFloat("light z", &fractal_state.fs_params.light_direction.z, -1.0f, 1.0f, NULL, 0);
        }
        igEnd();
    }
    simgui_render();
}