@echo off

cd src
zcc +cpm tetris.c -o ..\out\tetris_gp.com -O -pragma-output:noprotectmsdos -DSDCC -DGP -compiler sdcc -SO3
cd ..

