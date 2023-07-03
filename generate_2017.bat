@echo off

echo Updating submodules...
git submodule update --init --remote

echo Updating version...
call version.bat

echo Generating project files...
call "tools/premake5.exe" vs2017