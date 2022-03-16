#!/bin/bash
gcc-11 parallel-simulation.c -o ./executables/parallel -fopenmp

./executables/parallel
