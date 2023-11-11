-- premake5.lua
workspace "DrawBoard3D"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "DrawBoard3D"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "DrawBoard3D"