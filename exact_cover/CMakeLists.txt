cmake_minimum_required(VERSION 3.16)
project(exact_cover C)

set(CMAKE_C_STANDARD 99)

add_executable(exact_cover main.c Array2dInt.c arrayint.c listlikematrix.c listlikematrix.h exactcover.c exactcover.h greeklatinsquare.c permutations.c vectordef.c vectordef.h vectorfuncs.c vectorfuncs.h vector.c vector.h algorithms.c algorithms.h user_interactions.c)

