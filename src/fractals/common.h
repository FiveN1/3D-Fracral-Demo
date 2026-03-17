#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#include"sokol_app.h"
#include"sokol_gfx.h"
#include"sokol_glue.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include"cimgui.h"
#include"util/sokol_imgui.h"

#include"vecmath.h"

#include"shaders/include/shaders.h"

// common data for fractal implementations.

typedef struct fractal_t {
    float(*dist_func)(vec3_t);                              // distance function of fractal
    const sg_shader_desc* (*shader_desc_cb)(sg_backend);    // get shader description callback
    const char* name;                                       // name of fractal
} fractal_t;

typedef struct fractal_state_t {
    sg_bindings binings;
    sg_pipeline pipeline;
    vs_params_t vs_params;
    fs_params_t fs_params;
} fractal_state_t;

extern fractal_state_t fractal_state;

void fractal_state_init();
void fractal_state_set_fractal(fractal_t* fractal);
void fractal_state_draw();

#endif COMMON_H