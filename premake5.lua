workspace "Agata Engine"
	configurations { 
		"Debug", 
		"Release"
	}
	platforms { "x86", "x86_64"}

project "Agata"
	location "AgataEngine"
	language "C++"
	kind "ConsoleApp"
	cppdialect "C++17"

	targetdir "$(SolutionDir)BinOut/$(ProjectName)/$(Platform)/$(Configuration)"
	objdir "!$(SolutionDir)BinInt/$(ProjectName)/$(Platform)/$(Configuration)"

	files {
		"AgataEngine/**.cpp",
		"AgataEngine/**.h"
	}

	includedirs {
		"$(SolutionDir)Vendor/GLFW/$(Platform)/include",
		"$(SolutionDir)Vendor/GLEW/$(Platform)/include",
		"$(SolutionDir)Vendor/glm",
		"$(SolutionDir)Vendor/stb",
		"$(SolutionDir)Vendor/imgui",
		"$(SolutionDir)AgataEngine/Renderer",
		"$(SolutionDir)AgataEngine/Utils",
		"$(SolutionDir)AgataEngine/Scene",
		"$(SolutionDir)AgataEngine/Window",
		"$(SolutionDir)AgataEngine",
	}

	libdirs {
		"$(SolutionDir)Vendor/GLFW/$(Platform)/lib-vc2019",
		"$(SolutionDir)Vendor/GLEW/$(Platform)/lib"
	}

	links {
		"ImGui",
		"glfw3.lib",
		"glew32s.lib",
		"opengl32.lib",
		"kernel32.lib",
		"user32.lib",
		"gdi32.lib",
		"winspool.lib",
		"comdlg32.lib",
		"advapi32.lib",
		"shell32.lib",
		"ole32.lib",
		"oleaut32.lib",
		"uuid.lib",
		"odbc32.lib",
		"odbccp32.lib",
	}

	defines {
		"GLEW_STATIC"
	}

	filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

	filter "platforms:x86"
	  system "Windows"
	  architecture "x86"

  filter "platforms:x86_64"
	  system "Windows"
	  architecture "x86_64"


project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir "$(SolutionDir)BinOut/$(ProjectName)/$(Platform)/$(Configuration)"
	objdir "!$(SolutionDir)BinInt/$(ProjectName)/$(Platform)/$(Configuration)"

	files {
		"Vendor/imgui/imgui/imconfig.h",
		"Vendor/imgui/imgui/imgui.h",
		"Vendor/imgui/imgui/imgui.cpp",
		"Vendor/imgui/imgui/imgui_draw.cpp",
		"Vendor/imgui/imgui/imgui_internal.h",
		"Vendor/imgui/imgui/imgui_tables.cpp",
		"Vendor/imgui/imgui/imgui_widgets.cpp",
		"Vendor/imgui/imgui/imstb_rectpack.h",
		"Vendor/imgui/imgui/imstb_textedit.h",
		"Vendor/imgui/imgui/imstb_truetype.h",
		"Vendor/imgui/imgui/imgui_demo.cpp",
		"Vendor/imgui/imgui/imgui_impl_glfw.h", 
		"Vendor/imgui/imgui/imgui_impl_glfw.cpp",
		"Vendor/imgui/imgui/imgui_impl_opengl3.h",
		"Vendor/imgui/imgui/imgui_impl_opengl3.cpp"
	}

	includedirs {
		"$(SolutionDir)Vendor/GLFW/$(Platform)/include",
		"$(SolutionDir)Vendor/GLEW/$(Platform)/include"
	}

	defines {
		"GLEW_STATIC"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "platforms:x86"
        system "Windows"
        architecture "x86"

    filter "platforms:x86_64"
        system "Windows"
        architecture "x86_64"
