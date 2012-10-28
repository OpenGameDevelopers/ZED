echo Copying .lib files

MKDIR %ZEDPATH%\lib\Win32
XCOPY ..\..\..\lib\Win32 %ZEDPATH%\lib\Win32	/Y /D /E

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Win32
XCOPY .\Headers %ZEDPATH%\include\Win32 /Y /D
XCOPY ..\..\Common\Headers %ZEDPATH%\include\Win32 /Y /D