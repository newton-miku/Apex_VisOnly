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
title �ڲ�ר�õ�͸���������⴫V1.2.2
del /f /s /q Kdmapper.exe
cls
echo .
echo .
echo 	��ѡ������ϵͳ���
echo		1	win10	��Ĭ�ϣ�
echo		2	win11
echo 	������ϵͳ�汾�����
set /p ID1=

if ��%ID1%��==��1�� (
	goto win10
)else if ��%ID1%��==��2�� (
	goto win11
)else goto win10

:win11
echo win11
set "dir=%CD%\kdmapper\win11.exe"
set "goal=%CD%\"
if not exist "%dir%" (
	echo �ļ������ڣ������ļ������Ժ�����
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
	echo �ļ������ڣ������ļ������Ժ�����
	ECHO %dir%
	pause
	exit
)
copy "%dir%" "%goal%"
goto program

:program
ren *.exe Kdmapper.exe
echo ��ǰ����·���ǣ�%CD%
echo �ѻ�ȡ����ԱȨ��
echo 		������������
echo 		�������
Kdmapper.exe Driver.sys
echo ���������������
pause
cd program
ren *.exe Apex_VisOnly.exe
taskkill /F /T /IM NVIDIA Share.exe
start Apex_VisOnly.exe
"%NoRecoil%\ApexRW.ahk"
exit