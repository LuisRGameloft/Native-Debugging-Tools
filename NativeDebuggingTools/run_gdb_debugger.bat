@echo off
pushd %cd%\desktop\gdb
"%PYTHON_PATH%\python.exe" run_gdb_debugger.py
popd

