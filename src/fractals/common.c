#include"common.h"

fractal_state_t fractal_state = {
	.vs_params = {
		.eye_position = { 0.0, 0.0, 0.0 },
		.eye_direction = { 0.0, 0.0, -1.0 },
		.aspect = 1.0 // temp value
	},
	.fs_params = {
		.min_dist = 0.001f,
		.max_dist = 16.0f,
		.max_march_steps = 256,
		.frac_iterations = 8,
		.focal_length = 1.8f,

		.ambient_color =	{ 0.0f, 0.0f, 0.0f },
		.light_direction =	{ 1.0f, 1.0f, 1.0f },
		.light_color =		{ 1.0f, 1.0f, 1.0f },
		.spec_color =		{ 1.0f, 1.0f, 0.8f },
		.ambient_strength = 0.4f,
		.spec_strength =	0.4f,
		.spec_power =		10.0f,
	}
};

void fractal_state_init() {
	float fsq_verts[] = { -1.0f, -3.0f, 3.0f, 1.0f, -1.0f, 1.0f };
	fractal_state.binings = (sg_bindings){
		.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc) {
			.data = SG_RANGE(fsq_verts)
		}),
	};
}

void fractal_state_set_fractal(fractal_t* fractal) {
	sg_destroy_pipeline(fractal_state.pipeline); // note that this doesnt do anything if the pipeline doesnt exist.
	fractal_state.pipeline = sg_make_pipeline(&(sg_pipeline_desc) {
		.shader = sg_make_shader(fractal->shader_desc_cb(sg_query_backend())),
		.layout = (sg_vertex_layout_state){ .attrs[0].format = SG_VERTEXFORMAT_FLOAT2 }
	});
}

void fractal_state_draw() {
	sg_apply_pipeline(fractal_state.pipeline);
	sg_apply_bindings(&fractal_state.binings);
	sg_apply_uniforms(UB_vs_params, &SG_RANGE(fractal_state.vs_params));
	sg_apply_uniforms(UB_fs_params, &SG_RANGE(fractal_state.fs_params));
	sg_draw(0, 3, 1);
}