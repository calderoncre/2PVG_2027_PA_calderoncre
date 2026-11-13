-- Blackjack (2PA, practice 2). Generate with generate_vs.bat (or generate_xcode.sh).
-- API documentation: run doxygen in this folder (see Doxyfile).

workspace "Blackjack"
  configurations { "Debug", "Release" }
  location "build"
  cppdialect "C++20"
  includedirs { "include" }

  filter "system:windows"
    architecture "x86_64"

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

-- Your player, as a library others can link.
project "player"
  kind "StaticLib"
  files { "player/**" }

project "blackjack_console"
  kind "ConsoleApp"
  files { "include/**", "src/game/**", "src/console/**" }
  links { "player" }

project "blackjack_esat"
  kind "ConsoleApp"
  debugdir (_MAIN_SCRIPT_DIR)
  files { "include/**", "src/game/**", "src/esat/**" }
  includedirs { "deps/esat/include" }
  links { "player" }

  filter "system:windows"
    libdirs { "deps/esat/lib/win_x64" }
    links { "opengl32", "user32", "gdi32", "shell32" }
    linkoptions { "/IGNORE:4099" }  -- ESAT ships without .pdb

  filter { "system:windows", "configurations:Debug" }
    links { "ESAT_d" }

  filter { "system:windows", "configurations:Release" }
    links { "ESAT" }

  filter "system:macosx"
    libdirs { "deps/esat/lib/macos64" }
    links { "ESAT", "OpenGL.framework", "Cocoa.framework",
            "CoreVideo.framework", "IOKit.framework",
            "AudioToolbox.framework" }
