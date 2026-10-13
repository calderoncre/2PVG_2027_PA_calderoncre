@echo off
cls
echo ---------------------------------------------------
echo  ESAT Curso 2025-2026 Asignatura PRG Primero
echo ---------------------------------------------------
echo  Proceso por lotes iniciado.
echo ---------------------------------------------------

cl /nologo /Zi /GR- /EHs /MD /I"Lib_Graph\ESAT_rev248\include" main_graphics.cpp tiktaktoe.cpp game_interface.cpp igraphics.cpp "Lib_Graph\ESAT_rev248\bin\ESAT.lib" opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib /Fe:main_graphics.exe

echo ---------------------------------------------------
echo  Proceso por lotes finalizado.
echo ---------------------------------------------------
pause