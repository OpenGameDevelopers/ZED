echo Copying .lib files

MKDIR %ZEDPATH%\lib\Xbox
XCOPY ..\..\..\lib\Xbox D:\ZEDlib\lib\Xbox	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Xbox
XCOPY .\Headers D:\ZEDlib\include\Xbox /Y /D
XCOPY ..\..\Common\Headers D:\ZEDlib\include\Xbox /Y /D