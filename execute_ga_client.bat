@echo off

call config.bat

set CLIENT_FILE=ga-client.exe
set SERVER_IP=10.219.9.105
set SERVER_PORT=8554

cd %CONF_ROOT_DIR%
call %CLIENT_FILE% %CLIENT_CONF_FILE% rtsp://%SERVER_IP%:%SERVER_PORT%/desktop