echo Copying .lib files

MKDIR %ZEDPATH%\lib\Win64
XCOPY ..\..\..\lib\Win64 D:\ZEDlib\lib\Win64	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Win64
XCOPY .\Headers D:\ZEDlib\include\Win64 /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\include\Win64 /Y /D