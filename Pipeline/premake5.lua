project "Pipeline"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++11"
    characterset ("Unicode")

    targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/")
    objdir ("%{wks.location}/build/obj/" .. outputdir .. "/")

    files { 
        "src/**.h",
        "src/**.cpp",
    }
