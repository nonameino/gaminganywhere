@echo off

call config.bat

set SERVER_FILE=ga-server-event-driven.exe

call %CONF_ROOT_DIR%%SERVER_FILE% %SERVER_GAME_CONF_FILE%