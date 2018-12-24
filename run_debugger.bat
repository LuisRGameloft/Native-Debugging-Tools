@echo off

rem if exist config.bat
if not exist "config.bat" (
	echo You need to create config.bat using config_template file
	goto end
)


:end