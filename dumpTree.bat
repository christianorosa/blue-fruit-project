@echo off
setlocal ENABLEEXTENSIONS

REM ===============================
REM Configuración
REM ===============================
set OUTPUT=project_tree.txt
set ROOTDIR=%CD%

REM ===============================
REM Cabecera
REM ===============================
echo Project tree for: %ROOTDIR% > %OUTPUT%
echo Generated on: %DATE% %TIME% >> %OUTPUT%
echo. >> %OUTPUT%

REM ===============================
REM Generar árbol
REM ===============================
tree "%ROOTDIR%" /F /A >> %OUTPUT%

REM ===============================
REM Fin
REM ===============================
echo.
echo Tree written to %OUTPUT%
pause
