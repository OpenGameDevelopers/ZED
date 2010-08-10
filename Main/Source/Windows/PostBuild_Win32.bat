echo Copying .lib files

MKDIR %ZEDPATH%\lib\Win32
XCOPY ..\..\..\lib\Win32 D:\ZEDlib\lib\Win32	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Win32
XCOPY .\Headers D:\ZEDlib\include\Win32 /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\include\Win32 /Y /D