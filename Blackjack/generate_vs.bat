@echo off
rem Generates build\Blackjack.sln for Visual Studio.
"%~dp0tools\premake\windows\premake5.exe" --file="%~dp0premake5.lua" vs2026
