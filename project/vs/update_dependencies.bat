::
:: Updates the project's dependencies.
::
@echo off
setlocal

set thisDir=%~dp0
set depDir=%thisDir%..\..\deps

:: essentutils
rmdir %depDir%\essentutils /S /Q
set depProj=%thisDir%..\..\..\essentutils
call %depProj%\project\vs\deploy_as_sources.bat %depDir%
