#!/bin/bash
gcc-11 secuential-simulation.c -o ./executables/secuential -fopenmp

./executables/secuential
