cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 99)

#add_library(Vector STATIC Vector.c Vector.h vectordef.c vectordef.h vectorfuncs.c vectorfuncs.h)
set(VECTOR vector/arrayint.c
        vector/arrayint.h
        vector/Array2dInt.c
        vector/Array2dInt.h
        vector/vector.c
        vector/vector.h
        vector/vectordef.c
        vector/vectordef.h
        vector/vectorfuncs.c
        vector/vectorfuncs.h
        vector/algorithms.c
        vector/algorithms.h
)
