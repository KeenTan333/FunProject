#! /usr/bin/bash
mkdir -p ./Outputs
gcc src/"$1".c -std=c99 -o ./Outputs/"$1".exe $(sdl2-config --cflags --libs) -lm 2>&1 | tee ./Outputs/compilation.log