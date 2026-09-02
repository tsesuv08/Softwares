@ECHO OFF

:0

DEL C:\Users\Public\Desktop\*.* /s /q
DEL \\info\it0723$\Desktop\*.* /s /q

timeout /t 1 /nobreak

goto 0
