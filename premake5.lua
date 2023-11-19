toolset("v143")
systemversion("10.0.20348.0")

local function execute(command)
	local result, _ = os.outputof(command)

	return result
end

local target_extension = ".asi"

local function is_nfsco()
	local name = execute("echo %COMPUTERNAME%")

	return string.find(name, "ABOMINATOR")
end

local function set_defines()
	if is_nfsco() then
		defines("NFSCO")
	end
end

local function set_name()
	if is_nfsco() then
		filter "configurations:debug"
			targetname "%{prj.name}-debug"
	
		filter "configurations:release"
			targetname "%{prj.name}-release"
	
		filter "configurations:production"
			targetname "%{prj.name}"
	
		filter {}
	end
end

if is_nfsco() then
	target_extension = ".dll"
end

workspace "hyperlinked"
	location "build"
	startproject "hyperconsole"
	staticruntime "on"

	vectorextensions "sse2"
	largeaddressaware "on"

	editandcontinue "off"
	rtti "off"

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
		"nobuffersecuritycheck",
	}

	set_defines()

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

	symbols "full"

	filter "configurations:debug"
		defines {
			"DEBUG",
			"TRACE_HEAP_ALLOCATIONS",
			"CONCURRENT_POOL_ACCESS",
			"USE_HYPER_AABB_NONRECURSIVE",
			"USE_HYPER_VISIBILITY",
			"USE_HYPER_STREAMER"
		}
		optimize "debug"
		inlining "disabled"
		runtime "debug"

	filter "configurations:release"
		defines {
			"NDEBUG",
			"USE_SIMD_VECTORIZATIONS",
			"CONCURRENT_POOL_ACCESS",
			"USE_HYPER_AABB_NONRECURSIVE",
			"USE_HYPER_VISIBILITY",
			"USE_HYPER_STREAMER"
		}
		optimize "full"
		inlining "auto"
		runtime "release"

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

		targetextension(target_extension)

		postbuildcommands {
			"if \"%COMPUTERNAME%\" == \"ABOMINATOR\" ( copy /y \"$(TargetPath)\" \"D:\\Development\\nfsco\\nfsco\\install\\bin\\\" )",
		}

		set_name()

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
