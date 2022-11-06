@echo off
cd A:\Programieren
git add *
git commit -m "Automated commit on %date%"
git push -u origin main
