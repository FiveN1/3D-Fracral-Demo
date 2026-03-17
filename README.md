# 3D Fracral Demo
<div float="left">
  <img src="https://github.com/FiveN1/3D-Fracral-Demo/blob/main/res/screenshot1.png" alt="screenshot" width="256"/>
  <img src="https://github.com/FiveN1/3D-Fracral-Demo/blob/main/res/screenshot2.png" alt="screenshot" width="256"/>
</div>
<div float="left">
  <img src="https://github.com/FiveN1/3D-Fracral-Demo/blob/main/res/3D_fractal_demo_iteration_settings_24fps.gif" alt="screenshot" width="256"/>
  <img src="https://github.com/FiveN1/3D-Fracral-Demo/blob/main/res/screenshot4.png" alt="screenshot" width="256"/>
</div>

The demo is playable [here](https://fiven1.github.io/web/webgl_demos/sdf_fractal_demo/index.html) in the browser.
(warning! dont run this on your phone or it will freeze!)

## About the project
This project is very simple, and you can use it as reference material for developing your own SDF renderer.
The goal of this project was to learn about [signed distance fields](https://en.wikipedia.org/wiki/Signed_distance_function), and get an understanding of how are they rendered. In this project, I chose to use the [sphere tracing](https://en.wikipedia.org/wiki/Ray_marching) approach because it it is the easiest to implement, and is a good starting ground.

## How to build 
After cloning this repo you'll need to use premake to build the project files, either by running premake in the project, or by using the build.bat file, which calls premake from the premake_path defined in it (make sure that you set the correct premake_path!), then you can easily run the batch file and build the project by calling any of the premake commands (vs2022, gmake, ect... more in the help command) or by calling 'c' which compiles the program and runs it assuming that you have installed vs2022 in the exact location as I have :).

## Dependencies
After building your project, you might realise that it uses external dependencies, like sokol, vecmath & cimgui.
make sure to update the dependencies path correctly in the premake5.lua file. all dependencies are header only, except cimgui.
I recommend watching [Tim Milliards video](https://www.youtube.com/watch?v=DnbXLUDoS-4) about adding cimgui to your sokol project.
this project uses 3912b3d pull of imgui.

