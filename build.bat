@echo off
REM compile
color 08
C:/raylib/w64devkit/bin/mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=src/*.cpp
REM run
color 03
REM main.exe
REM clear
color 07