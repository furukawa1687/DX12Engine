@echo off

pushd "%~dp0"
call @clean.bat
popd

::----------------------------------------------------
:: プロジェクトファイルを生成
::----------------------------------------------------
bin\premake5.exe vs2019 --file=bin/premake5.lua

if %ERRORLEVEL% neq 0 (
	echo.
	echo エラーが発生しました。Luaスクリプトを確認してください。
	pause
)
