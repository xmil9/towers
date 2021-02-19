::
:: Deploys the game's content to a given location.
::
@echo off
setlocal

set destDir=%~1
set thisDir=%~dp0

set resSrc=%thisDir%..\..\resources
set resDest=%destDir%\resources
mkdir "%resDest%"
robocopy "%resSrc%" "%resDest%" *.json /s
robocopy "%resSrc%" "%resDest%" *.png /s

set fontsSrc=%resSrc%\fonts
set fontsDest=%resDest%\fonts
mkdir "%fontsDest%"
robocopy "%fontsSrc%" "%fontsDest%" *.ttf

set shaderSrcA=%thisDir%..\..\shaders
set shaderSrcB=%thisDir%..\..\deps\spiel\shaders
set shaderDest=%destDir%\shaders
mkdir "%shaderDest%"
robocopy "%shaderSrcA%" "%shaderDest%" *.*
robocopy "%shaderSrcB%" "%shaderDest%" *.*

exit 0
