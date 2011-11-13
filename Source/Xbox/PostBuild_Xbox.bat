echo Copying .lib files

MKDIR %ZEDPATH%\lib\Xbox
%SystemRoot%\system32\xcopy.exe ..\..\..\lib\Xbox %ZEDPATH%\lib\Xbox	/Y /D

echo Copying .hpp files

MKDIR %ZEDPATH%\include\Xbox
%SystemRoot%\system32\xcopy.exe .\Headers %ZEDPATH%\include\Xbox /Y /D
%SystemRoot%\system32\xcopy.exe ..\..\Common\Headers %ZEDPATH%\include\Xbox /Y /D