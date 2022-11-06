@echo off
cd ../
git add *
git commit -m "automated commit on %date%"
git push -u origin main
