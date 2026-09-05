@echo off

cd src
zcc +cpm tetris.c -o ..\out\tetris_altair.com -O -pragma-output:noprotectmsdos -clib=8080 -DZCC -DALTAIR8800
cd ..
