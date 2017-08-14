#!/bin/bash
all:	mat_mult.c	read.c	read.h
	gcc -pg -fopenmp -o exe mat_mult.c read.c read.h
clean:
	$(RM) exe
