@echo off

echo Updating submodules...
git submodule update --init --recursive --depth 1 --jobs=16

echo Generating project files...
call "tools/premake5.exe" vs2022