cmake_minimum_required(VERSION 3.16)
project(mpk C)

set(CMAKE_C_STANDARD 11)

set(LINEAR_ALGEBRA linear_algebra_elements/matrix.c
        linear_algebra_elements/matrix.h
        linear_algebra_elements/vector_algebraic_addendum.c
        linear_algebra_elements/vector_algebraic_addendum.h
        linear_algebra_elements/rational.c
        linear_algebra_elements/rational.h)