@echo off
rem Generates the API documentation in doc\html (open doc\html\index.html).
rem The first time it downloads a portable doxygen into tools\doxygen\windows.
setlocal
set VERSION=1.18.0
set DIR=%~dp0tools\doxygen\windows
if not exist "%DIR%\doxygen.exe" (
  echo Downloading doxygen %VERSION%...
  if not exist "%DIR%" mkdir "%DIR%"
  "%SystemRoot%\System32\curl.exe" -fL -o "%DIR%\doxygen.zip" https://github.com/doxygen/doxygen/releases/download/Release_%VERSION:.=_%/doxygen-%VERSION%.windows.x64.bin.zip || exit /b 1
  "%SystemRoot%\System32\tar.exe" -xf "%DIR%\doxygen.zip" -C "%DIR%" doxygen.exe || exit /b 1
  del "%DIR%\doxygen.zip"
)
cd /d "%~dp0"
"%DIR%\doxygen.exe" Doxyfile || exit /b 1
echo Done: open doc\html\index.html
