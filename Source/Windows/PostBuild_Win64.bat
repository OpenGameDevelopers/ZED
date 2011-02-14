echo Copying .lib files

MKDIR %ZEDPATH%\lib\Win64
XCOPY ..\..\..\lib\Win64 %ZEDPATH%\lib\Win64	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Win64
XCOPY .\Headers %ZEDPATH%\include\Win64 /Y /D
XCOPY ..\..\Common\Headers %ZEDPATH%\include\Win64 /Y /D