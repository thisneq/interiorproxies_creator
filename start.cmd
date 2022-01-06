del %~dp0output\fxmanifest.lua
del %~dp0output\interiorproxies.meta
cd src
node app.js
echo off
cls
start %~dp0output