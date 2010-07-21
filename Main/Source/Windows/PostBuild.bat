echo Copying .lib files

XCOPY ..\..\..\lib\Win32 D:\ZEDlib\Win32\lib	/Y /D

echo Copying .hpp files

XCOPY .\Headers D:\ZEDlib\Win32\include /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\Win32\include /Y /D