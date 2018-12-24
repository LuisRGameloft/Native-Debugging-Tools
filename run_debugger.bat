@echo off
set CUR_PATH=%cd%

rem Check if exist config.bat file
if not exist "config.bat" (
	echo You need to create config.bat using config_template file
	goto end
)
call %CUR_PATH%\config.bat
pushd %CUR_PATH%\NativeDebuggingTools
call run_gdb_debugger.bat
popd

:end