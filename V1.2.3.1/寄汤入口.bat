@echo off
color 2e
if exist "%SystemRoot%\SysWOW64" path %path%;%windir%\SysNative;%SystemRoot%\SysWOW64;%~dp0
bcdedit >nul
if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)
:UACPrompt
%1 start "" mshta vbscript:createobject("shell.application").shellexecute("""%~0""","::",,"runas",1)(window.close)&exit
exit /B
:UACAdmin
cd /d "%~dp0"
title 内部专用单透——不得外传V1.2.2
del /f /s /q Kdmapper.exe
cls
echo .
echo .
echo 	请选择您的系统序号
echo		1	win10	（默认）
echo		2	win11
echo 	请输入系统版本的序号
set /p ID1=

if “%ID1%”==“1” (
	goto win10
)else if “%ID1%”==“2” (
	goto win11
)else goto win10

:win11
echo win11
set "dir=%CD%\kdmapper\win11.exe"
set "goal=%CD%\"
if not exist "%dir%" (
	echo 文件不存在，请检查文件完整性后重试
	ECHO %dir%
	pause
	exit
)
copy "%dir%" "%goal%"
goto program

:win10
echo win10
set "dir=%CD%\kdmapper\win10.exe"
set "goal=%CD%\"
if not exist "%dir%" (
	echo 文件不存在，请检查文件完整性后重试
	ECHO %dir%
	pause
	exit
)
copy "%dir%" "%goal%"
goto program

:program
ren *.exe Kdmapper.exe
echo 当前运行路径是：%CD%
echo 已获取管理员权限
echo 		啊，哈哈哈哈
echo 		寄汤来喽
Kdmapper.exe Driver.sys
echo 按任意键启动程序
pause
cd program
ren *.exe Apex_VisOnly.exe
taskkill /F /T /IM NVIDIA Share.exe
start Apex_VisOnly.exe
"%NoRecoil%\ApexRW.ahk"
exit