#!/bin/bash
mkdir -p ./Outputs
gcc src/"$1".c -o ./Outputs/"$1".exe 2>&1 | tee ./Outputs/compilation.log