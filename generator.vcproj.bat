@echo off
REM need to do the 'cd' at the beginning, or my python script won't find the config file.

echo 1. set the current working directory to the dir that the bat file is in.
for /f %%i in ('cd') do set CWD=%%i


c:\Qt\4.3.3\bin\qmake.exe generator.pro -tp vc -spec win32-msvc2005 -r