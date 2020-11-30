workspace "Kursach"
    configurations { 
        "Debug",
        "Release"
    }  
    configurations { "Debug", "Release" }

    platforms { "x86", "x86_64" }

    startproject "Pipeline"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Pipeline/"