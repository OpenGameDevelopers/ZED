echo Copying .lib files

MKDIR D:\ZEDlib\Win32\lib
XCOPY ..\..\..\lib\Win32 D:\ZEDlib\Win32\lib	/Y /D

echo Copying .hpp files

MKDIR D:\ZEDlib\Win32\include
XCOPY .\Headers D:\ZEDlib\Win32\include /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\Win32\include /Y /D