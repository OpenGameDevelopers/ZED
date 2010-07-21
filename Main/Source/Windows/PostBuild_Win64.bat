echo Copying .lib files

MKDIR D:\ZEDlib\Win64\lib
XCOPY ..\..\..\lib\Win64 D:\ZEDlib\Win64\lib	/Y /D

echo Copying .hpp files

MKDIR D:\ZEDlib\Win64\include
XCOPY .\Headers D:\ZEDlib\Win64\include /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\Win64\include /Y /D