cmake_minimum_required(VERSION 3.16)
project(fibonachy_nim C)

set(CMAKE_C_STANDARD 99)

                              #add_executable(exact_cover main.c ../vector/Array2dInt.c ../vector/arrayint.c listlikematrix.c listlikematrix.h exactcover.c exactcover.h greeklatinsquare.c permutations.c ../vector/vectordef.c ../vector/vectordef.h ../vector/vectorfuncs.c ../vector/vectorfuncs.h ../vector/vector.c ../vector/vector.h algorithms.c algorithms.h user_interactions.c)
set(EXACT_COVER exact_cover/exactcover.c exact_cover/exactcover.h exact_cover/listlikematrix.c exact_cover/listlikematrix.h)

