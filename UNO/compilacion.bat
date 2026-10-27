@echo off
cls
pushd "%~dp0"

echo ---------------------------------------------------
echo  Compilando UNO
echo ---------------------------------------------------

cl /nologo /Zi /GR- /EHs /MD ^
 /I"Lib_Graph\ESAT_rev248\include" ^
 *.cpp ^
 "Lib_Graph\ESAT_rev248\bin\ESAT.lib" ^
 opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib ^
 /Fe:UNO.exe

if errorlevel 1 (
    echo ERROR: La compilacion ha fallado.
) else (
    echo Compilacion completada correctamente.
)

popd
pause