@echo off
cls

REM     Build Script

REM Set Compiler Settings Here


set GCC=gcc
set AR=ar
set OUTPUT=libgumbo_parser.a
set DEBUGMODE=0

set REBUILD_LIBRARIES=0

set LINK_ONLY=0
set VERBOSE=0

set ASYNC_BUILD=1

set C_COMPILER_FLAGS=-std=c99 -Wall
set ADDITIONAL_LIBRARIES=
set ADDITIONAL_LIBDIRS=
set ADDITIONAL_INCLUDEDIRS=-I.


del %OUTPUT% 2>nul

setlocal enabledelayedexpansion


if %LINK_ONLY% GTR 0 (
    goto linker
)

if %DEBUGMODE% GTR 0 (
    set DEBUG_INFO=-g
) else (
    set DEBUG_INFO=
)

if %ASYNC_BUILD% GTR 0 (
    set WAIT=
) else (
    set WAIT=/WAIT
)

if %REBUILD_LIBRARIES% GTR 0 (
    del /S /Q "*.o" 2>nul
)

echo Building C Libraries...
for %%F in (*.c) do (
    if not exist %%~nF.o (
        echo Building %%~nF.o
        start /B %WAIT% "%%~nF.o" %GCC% %ADDITIONAL_INCLUDEDIRS% %C_COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o %%~nF.o

        if %VERBOSE% GTR 0 (
            echo %GCC% %ADDITIONAL_INCLUDEDIRS% %C_COMPILER_FLAGS% %DEBUG_INFO% -c %%F -o %%~nF.o
        )
    )
)

REM Wait for building process to finish
:loop
set /a count=0
for /f %%G in ('tasklist ^| find /c "%GCC%"') do ( set /A count=%count%+%%G )
if %count%==0 (
    goto linker
) else (
    timeout /t 2 /nobreak>nul
    goto loop
)

:linker

set "files="
for /f "delims=" %%A in ('dir /b /a-d "*.o" ') do set "files=!files! %%A"

:link
echo Linking Executable...

if %VERBOSE% GTR 0 (
    echo %AR% ru %OUTPUT% %files%
)

%AR% ru %OUTPUT% %files%

:finish
if exist .\%OUTPUT% (
    echo Build Success!
) else (
    echo Build Failed!
)