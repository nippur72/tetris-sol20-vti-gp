@echo off

cd src
zcc +sol20 -pragma-define:REGISTER_SP=16383 -pragma-define CRT_ORG_CODE=256 tetris.c -o ..\out\tetris_sol20.com -DZCC -DSOL20 -create-app
cd ..
call node tools\mkent out\tetris_sol20.com 0100 > out\tetris_sol20.ent

