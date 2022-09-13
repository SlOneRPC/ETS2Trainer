workspace "ETS2Internal"
    platforms { "x64" }
    architecture "x64"


    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "ETS2Internal"
    location "src"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/build/%{cfg.buildcfg}")
    objdir ("bin/obj/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp",
        "src/**.cc",
        "deps/IMGUI/**.cpp"
    }

    includedirs
    {
        "$(SolutionDir)deps/IMGUI",
        "$(SolutionDir)deps/MinHook"
    }

    libdirs { "deps/MinHook", "$(DXSDK_DIR)Lib/x64" }

    filter "system:windows"
        cppdialect "C++20"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        staticruntime "on"
        runtime "Debug"
        links { "libMinHook-x64.lib", "dxguid.lib" }
  
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        staticruntime "on"
        links {"libMinHook-x64.lib", "dxguid.lib" }
