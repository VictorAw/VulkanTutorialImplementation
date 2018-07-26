@echo off

if "%~1" == "" (
  echo;
  echo "Please specify a project to build"
  echo;
  exit
)

SET PROJECT=%1

if %PROJECT% == all (
  if exist .\bin (
    del /s /q .\bin
    rmdir /s /q .\bin
  )

  for /D %%a in (*) do (
    if exist %%a\build (
      del /s /q %%a\build
      rmdir /s /q %%a\build
    )
  )
) else (
  if not exist .\%PROJECT% (
    echo;
    echo "Provided project name '%PROJECT%' does not exist."
    echo;
    exit
  )
  else (
    if exist .\bin\Debug\%PROJECT%.exe (
      del /s /q .\bin\Debug\%PROJECT%.exe
    )

    if exist .\bin\Release\%PROJECT%.exe (
      del /s /q .\bin\Release\%PROJECT%.exe
    )

    if exist .\%PROJECT%\build (
      del /s /q .\%PROJECT%\build
      rmdir /s /q .\%PROJECT%\build
    )
  )
)

