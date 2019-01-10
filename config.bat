@echo off

set ROOT=%~dp0
set CONF_ROOT_DIR=%ROOT%\ga\vs2012\x64\Debug\
REM set SERVER_GAME_CONF_FILE=%CONF_ROOT_DIR%config\Config_Port_51691_D11.conf
REM set SERVER_GAME_CONF_FILE=%CONF_ROOT_DIR%config\Config_Port_51691_A9_1280.conf
set SERVER_GAME_CONF_FILE=%CONF_ROOT_DIR%config\server.d3dex.conf
set CLIENT_CONF_FILE=%CONF_ROOT_DIR%config\client.rel.conf