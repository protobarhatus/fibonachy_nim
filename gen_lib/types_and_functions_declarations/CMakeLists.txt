cmake_minimum_required(VERSION 3.16)
project(mpk C)

set(CMAKE_C_STANDARD 11)

set(TYPES_AND_FUNCS         types_and_functions_declarations/type_declaration.c
        types_and_functions_declarations/type_declaration.h
        types_and_functions_declarations/arithmetic_type_declaration.c
        types_and_functions_declarations/arithmetic_type_declaration.h
        types_and_functions_declarations/functions_extensions.c
        types_and_functions_declarations/functions_extensions.h)