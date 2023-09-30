@echo off
set PLAT="32"
IF "%Platform%"=="x64" set PLAT="64"

echo =================================================
echo Building for Platform: %PLAT%

cl runshc.c /O2 /nologo /MD /link kernel32.lib user32.lib advapi32.lib /MANIFEST:NO /ENTRY:"main" /SUBSYSTEM:WINDOWS /out:runshc%PLAT%.exe