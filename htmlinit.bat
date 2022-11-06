@echo off
if exist %1\ (
    type nul > %1\site.html
    type nul > %1\script.js
    type nul > %1\style.css
) else (
    mkdir %1
    type nul > %1\site.html
    type nul > %1\script.js
    type nul > %1\style.css
)