@echo off
A:
cd A:\Programieren
git add *
git commit -m "Automated commit on %date%"
git push
pause
