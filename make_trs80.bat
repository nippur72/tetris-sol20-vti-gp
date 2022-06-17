@echo off

cd src
zcc +trs80 tetris.c -o ..\out\tetris_trs80.com -DZCC -DTRS80 -create-app
cd ..

