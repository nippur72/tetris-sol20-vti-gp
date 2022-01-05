@echo off

cd src
zcc +sol20 -pragma-define CRT_ORG_CODE=256 tetris.c -o ..\out\tetris_sol20.com -DZCC -DSOL20 -create-app
cd ..

