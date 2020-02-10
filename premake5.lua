workspace "WalkingThresh"
   configurations { "Debug", "Release" }
   location "vs"

project "WalkingThresh"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin/obj/%{cfg.buildcfg}"

   files { "src/**.hpp", "src/**.cpp" }
   includedirs { "src" }
   libdirs { "src/glut" }
   links { "glut32", "opengl32" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"