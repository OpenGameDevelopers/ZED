echo Copying .dll files

MKDIR %ZEDPATH%\lib\XNA
Get-ChildItem -Path ..\..\..\Lib\XNA\Debug\*.dll | Rename-Item -newname {"ZED" + $_.name -replace ".dll", "D.dll"}
XCOPY ..\..\..\lib\XNA\Debug D:\ZEDlib\lib\XNA	/Y /D