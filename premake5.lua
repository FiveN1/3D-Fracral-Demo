
output_directory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

workspace "3d_fractal_demo"
	startproject "3d_fractal_demo"

	system "windows"

	configurations {
        "Debug",
        "Release"
    }

	filter "system:windows"
		architecture "x86_64"
		buildoptions {
			"-mwin32" -- pro mingw
		}
		linkoptions {
			"-lkernel32",
			"-luser32",
			"-lshell32",
			"-lgdi32" -- pro gl backend
		}
	filter "system:emscripten"
		architecture "wasm64"
		-- link options pro emscripten
		linkoptions {
			"-s ALLOW_MEMORY_GROWTH=1",
			-- sokol
			"-s USE_WEBGL2=1",
			-- output
			--"-o %{wks.location}/bin/" .. output_directory .."/%{prj.name}/index.html"
			"-o index.html"
		}

c_libs_directory = "C:/C/libraries"

project "3d_fractal_demo"
	kind "ConsoleApp"
	
	--language "C" -- když linkuješ c++ projekty, jazyk musí být c++, používá se g++
	cdialect "C99"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	
    targetdir ("%{wks.location}/bin/" .. output_directory .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. output_directory .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.c"
	}
	
	includedirs {
		"src",
		c_libs_directory .. "/sokol",
		c_libs_directory .. "/vecmath",
		c_libs_directory .. "/cimgui"
	}

	links {
		"cimgui"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"AL_PLATFORM_WINDOWS",
			"SOKOL_GLCORE"
		}
	filter "system:linux"
		systemversion "latest"
		defines {
			"AL_PLATFORM_LINUX",
			"SOKOL_GLCORE"
		}
	filter "system:macosx"
		systemversion "latest"
		compileas "Objective-C"
		defines {
			"AL_PLATFORM_MACOSX",
			"SOKOL_GLCORE" -- "SOKOL_METAL"
		}
	filter "system:emscripten"
		systemversion "latest"
		defines {
			"AL_PLATFORM_EMSCRIPTEN",
			"SOKOL_GLES3" -- "or SOKOL_WGPU"
		}
	filter "system:android"
		systemversion "latest"
		defines {
			"AL_PLATFORM_ANDROID",
			"SOKOL_GLES3"
		}
	filter "system:ios"
		systemversion "latest"
		defines {
			"AL_PLATFORM_IOS",
			"SOKOL_GLES3"
		}

	filter "configurations:Debug" 
		defines {
			"AL_DEBUG",
			"SOKOL_DEBUG"
		}
		symbols "on"
	filter "configurations:Release" 
		defines "AL_RELEASE"
		optimize "on"

group "build_libs"
	include "C:/C/libraries/cimgui"
group ""

-- vycisti projekt od kompilovanych souboru.
newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate binaries, and vs files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate binaries")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Removing include directory")
        os.rmdir("./include")
        print("Done")
    end
}