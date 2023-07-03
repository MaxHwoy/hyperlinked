@echo off

for /f %%i in ('git rev-list --count head') do set version=%%i

echo #define __VERSION__ %version% > src\\version.hpp

echo Version is %version%.