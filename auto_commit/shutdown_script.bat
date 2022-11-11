@echo off
A:
cd A:\Programieren
git pull
git add *
git commit -m "Automated commit on %date%"
git push
