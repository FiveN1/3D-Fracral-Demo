@echo off

set shdc_exe="C:/C/libraries/sokol/shdc/bin/win32/sokol-shdc.exe"
set shader_directory=src/shaders

:Done

echo.
echo Enter a shader that you want to compile in "%shader_directory%/src":
echo.

set /p user_input=

::IF "%user_input%" == "y"         GOTO Compile

::echo.
::echo shaders not compiled
::echo.

::for %f in (.\*) do @echo %f

::GOTO Done

::
:: Compile
::

:Compile

%shdc_exe% --input %shader_directory%/src/%user_input%.glsl --output  %shader_directory%/include/%user_input%.h --slang glsl430:hlsl5:metal_macos:glsl300es

echo.
echo shaders compiled at: %shader_directory%/include/%user_input%.h
echo.

GOTO Done