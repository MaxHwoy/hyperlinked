toolset("v143")
systemversion("10.0.20348.0")

workspace "hyperlinked"
	location ".\\build\\"
	startproject "hyperconsole"
	vectorextensions "sse2"
	largeaddressaware "on"

	targetdir "%{wks.location}\\bin\\%{cfg.buildcfg}\\"
	objdir "%{wks.location}\\obj\\%{cfg.buildcfg}\\%{prj.name}\\"
	buildlog "%{wks.location}\\obj\\%{cfg.buildcfg}\\%{prj.name}.log"

	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "mbcs"
	warnings "extra"

	includedirs {
		"src/",
	}

	buildoptions {
		"/Zm200",
		"/utf-8",
		"/std:c++latest",
	}

	flags {
		"no64bitchecks",
		"shadowedvariables",
		"noincrementallink",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	defines {
		"NOMINMAX",
		"_CRT_SECURE_NO_WARNINGS",
		"WIN32_LEAN_AND_MEAN",
		"_USE_MATH_DEFINES",
	}

	platforms {
		"x86",
		"x64",
	}

	configurations {
		"debug",
		"release",
	}

	filter "configurations:debug"
		defines "DEBUG"
		optimize "debug"
		inlining "disabled"
		runtime "debug"
		symbols "full"

	filter "configurations:release"
		defines "NDEBUG"
		optimize "full"
		inlining "auto"
		runtime "release"
		symbols "full"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"
		defines "TARGET_64BIT"

	project "hyperlib"
		language "c++"
		kind "staticlib"

		-- pchheader "stdafx.hpp"
		-- pchsource "src/hyperlib/stdafx.cpp"

		files {
			"src/hyperlib/**.*",
		}

		includedirs {
			"src/hyperlib",
			"thirdparty/dxsdk/include",
		}

		libdirs("thirdparty/dxsdk/lib/x86")

		links {
			"d3d9",
			"d3dx9"
		}

	project "hyperconsole"
		language "c++"
		kind "consoleapp"

		-- pchheader "stdafx.hpp"
		-- pchsource "src/hyperconsole/stdafx.cpp"

		files {
			"src/hyperconsole/**.*",
		}

		includedirs {
			"src/hyperconsole",
			"thirdparty/dxsdk/include",
		}

		libdirs("thirdparty/dxsdk/lib/x86")

		links {
			"hyperlib",
			"d3d9",
			"d3dx9"
		}

	project "hyperlinked"
		language "c++"
		kind "sharedlib"
		targetextension ".asi"

		-- pchheader "stdafx.hpp"
		-- pchsource "src/hyperlinked/stdafx.cpp"

		files {
			"src/hyperlinked/**.*",
		}

		includedirs {
			"src/hyperlinked",
			"thirdparty/dxsdk/include",
		}

		libdirs("thirdparty/dxsdk/lib/x86")

		links {
			"hyperlib",
			"d3d9",
			"d3dx9"
		}
