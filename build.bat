@echo off

SETLOCAL EnableDelayedExpansion

if "%~1" == "" (
  echo;
  echo "Please specify a project to build"
  echo;
  exit
)

SET PROJECT=%1

if not exist .\%PROJECT% (
  echo;
  echo "Provided project name '%PROJECT%' does not exist."
  echo;
  exit
) else (
  if %PROJECT:~-1%==\ SET PROJECT=%PROJECT:~0,-1%
  if %PROJECT:~-1%==/ SET PROJECT=%PROJECT:~0,-1%
)

if "%~2" == "" (
  SET MODE=Release
) ELSE IF "%~2" == "Debug" (
  SET MODE=Debug
) ELSE IF "%~2" == "DEBUG" (
  SET MODE=Debug
) ELSE IF "%~2" == "debug" (
  SET MODE=Debug
) ELSE (
  SET MODE=Release
)

SET LIBS_DIR=C:\Programming\C++\Libraries

SET GLFW=%LIBS_DIR%\glfw
SET GLFW_LIBS_DIR=%GLFW%\lib\%MODE%
SET GLFW_INCLUDE_DIR=%GLFW%\include
SET GLFW_LIBS=glfw3dll.lib
SET GLFW_DLLS=glfw3.dll

SET GLM_INCLUDE_DIR=%LIBS_DIR%\glm

SET VULKAN=C:\VulkanSDK\1.1.73.0
SET VULKAN_LIBS_DIR=%VULKAN%\Lib32
SET VULKAN_INCLUDE_DIR=%VULKAN%\Include
SET VULKAN_LIBS=vulkan-1.lib

SET INCLUDE_DIR=%PROJECT%\include
SET SRC_DIR=%PROJECT%\src
SET SRCS=%SRC_DIR%\*.cpp

SET SYSTEM_LIBS=opengl32.lib user32.lib kernel32.lib gdi32.lib
SET LIBS=%SYSTEM_LIBS% %GLFW_LIBS% %VULKAN_LIBS%

echo %MODE%
echo %SRCS%

SET CFLAGS=-I %INCLUDE_DIR%
SET CFLAGS=%CFLAGS% -I %GLFW_INCLUDE_DIR%
SET CFLAGS=%CFLAGS% -I %GLM_INCLUDE_DIR%
SET CFLAGS=%CFLAGS% -I %VULKAN_INCLUDE_DIR%
SET CFLAGS=%CFLAGS% -EHsc
SET CFLAGS=%CFLAGS% -analyze
SET CFLAGS=%CFLAGS% -W4
SET CFLAGS=%CFLAGS% -Zc:inline
SET CFLAGS=%CFLAGS% -errorReport:prompt
SET CFLAGS=%CFLAGS% -nologo
SET CFLAGS=%CFLAGS% -D WIN32
if %MODE% == Release (
  echo Setting Release Flags
  SET CFLAGS=!CFLAGS! -GL
  SET CFLAGS=!CFLAGS! -Gy
  SET CFLAGS=!CFLAGS! -O2
  SET CFLAGS=!CFLAGS! -D NDEBUG
  SET CFLAGS=!CFLAGS! -MD
) else (
  echo Setting Debug Flags
  SET CFLAGS=!CFLAGS! -Od
  SET CFLAGS=!CFLAGS! -RTC1 
  SET CFLAGS=%CFLAGS% -Zi
  SET CFLAGS=!CFLAGS! -MDd
)
SET CFLAGS=%CFLAGS% -Oy-
echo;
echo %CFLAGS%
echo;

cl %CFLAGS% ^
%SRCS% ^
-link ^
-libpath:%GLFW_LIBS_DIR% ^
-libpath:%VULKAN_LIBS_DIR% ^
%LIBS% ^
-out:%PROJECT%.exe

SET BIN=.\bin
SET BUILD=.\%PROJECT%\build\%MODE%

if not exist %BIN% (
  md %BIN%
)

if not exist %BUILD% (
  md %BUILD%
)

if not exist %BIN%\%MODE% (
  md %BIN%\%MODE%
)

if exist %BIN%\%MODE%\%PROJECT%.exe (
  del %BIN%\%MODE%\%PROJECT%.exe
)
if exist %PROJECT%.exe (
  move /y %PROJECT%.exe %BIN%\%MODE%\%PROJECT%.exe
)
move /y *.obj %BUILD%
move /y *.ilk %BUILD%
move /y *.idb %BUILD%
move /y *.pdb %BUILD%
move /y *.xml %BUILD%
for %%a in (%GLFW_DLLS%) do (
  if not exist %BIN%\%MODE%\%%a (
    copy "%GLFW_LIBS_DIR%\%%a" "%BIN%\%MODE%"
  )
)

