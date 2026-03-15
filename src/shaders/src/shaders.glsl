// -- ctype
@ctype vec3 vec3_t

// --- include
@include vs.glsl
@include fractal_funcs.glsl
@include fs.glsl

// in here we create one shader for every fractal type.

// web fractal
@fs fs1
@include_block web_fractal
@include_block fs_block
@end
@program web_fractal_shd vs fs1

// stronghold fractal
@fs fs2
@include_block stronghold_fractal
@include_block fs_block
@end
@program stronghold_fractal_shd vs fs2

// mandelbulb fractal
@fs fs3
@include_block mandelbulb_fractal
@include_block fs_block
@end
@program mandelbulb_fractal_shd vs fs3

// warping cubes fractal
@fs fs4
@include_block warping_cubes_fractal
@include_block fs_block
@end
@program warping_cubes_fractal_shd vs fs4

// tube fractal
@fs fs5
@include_block tube_fractal
@include_block fs_block
@end
@program tube_fractal_shd vs fs5

// monolith fractal
@fs fs6
@include_block monolith_fractal
@include_block fs_block
@end
@program monolith_fractal_shd vs fs6

// tdhooper1 fractal
@fs fs7
@include_block tdhooper1_fractal
@include_block fs_block
@end
@program tdhooper1_fractal_shd vs fs7

// yonatan fractal
@fs fs8
@include_block yonatan_fractal
@include_block fs_block
@end
@program yonatan_fractal_shd vs fs8