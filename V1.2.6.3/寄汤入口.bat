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
title �ڲ�ר�õ�͸���������⴫V1.2
cls
echo ��ǰ����·���ǣ�%CD%
echo �ѻ�ȡ����ԱȨ��
echo 		������������
echo 		�������
cd program
ren *.exe Apex_VisOnly.exe
IF EXIST "%PROGRAMFILES%\AutoHotkey\AutoHotkey.exe" (
	set AHK_PATH=%PROGRAMFILES%\AutoHotkey\AutoHotkey.exe
)
IF EXIST "%NoRecoil%\2Exe.bat" (
	START /B /W "" "%NoRecoil%\2Exe.bat"
	)
if exist %NoRecoil%\"START.exe" (
	START /B "" "%NoRecoil%\ApexRW.exe"
	) ELSE (
	START /B "%AHK_PATH%" "%NoRecoil%\ApexRW.ahk"
	)
start Apex_VisOnly.exe
EXIT