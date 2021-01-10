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
robocopy "%resSrc%" "%resDest%" *.json
robocopy "%resSrc%" "%resDest%" *.png

set fontsSrc=%resSrc%\fonts
set fontsDest=%resDest%\fonts
mkdir "%fontsDest%"
robocopy "%fontsSrc%" "%fontsDest%" *.ttf

set shaderSrc=%thisDir%..\..\shaders
set shaderDest=%destDir%\shaders
mkdir "%shaderDest%"
robocopy "%shaderSrc%" "%shaderDest%" *.*
robocopy "%shaderSrc%" "%shaderDest%" *.*

exit 0
