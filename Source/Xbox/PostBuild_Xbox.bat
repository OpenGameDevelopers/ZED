echo Copying .lib files

MKDIR %ZEDPATH%\lib\Xbox
XCOPY ..\..\..\lib\Xbox %ZEDPATH%\lib\Xbox	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Xbox
XCOPY .\Headers %ZEDPATH%\include\Xbox /Y /D
XCOPY ..\..\Common\Headers %ZEDPATH%\include\Xbox /Y /D