set RUNDIR=%~dp0
set BASEDIR=%RUNDIR%..
%RUNDIR%utm.exe delete %1 --LogPath %BASEDIR%\l --LogPrefix daemon-transport
