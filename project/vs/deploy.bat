::
:: Deploys the game's content to a given location.
::
@echo off
setlocal

set destDir=%~1
set thisDir=%~dp0
set resSrcDir=%thisDir%..\..\resources
set resDestDir=%destDir%\resources

mkdir "%resDestDir%"
robocopy %resSrcDir% %resDestDir% *.json
robocopy %resSrcDir% %resDestDir% *.png

exit 0
