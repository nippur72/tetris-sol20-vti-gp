@echo off

cd src
zcc +sol20 -crt0=sol20_crt0.asm -pragma-define CRT_ORG_CODE=256 tetris.c -o ..\out\tetris_sol20.com -DZCC -DSOL20 -create-app
cd ..

