cmake_minimum_required(VERSION 3.16)
project(fibonachy_nim C)

set(CMAKE_C_STANDARD 99)

include(exact_cover/CMakeLists.txt)
include(greeklatinsquare/CMakeLists.txt)
include(gen_lib/CMakeLists.txt)

add_executable(fib_lvl_1 fib\ lvl1.c)
add_executable(fib_lvl_2 fib\ lvl\ 2.c)
add_executable(greeklatinsquares greeklatinsq.c ${TYPES_AND_FUNCS} ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(exact_cover_solv exact_cover_main.c ${TYPES_AND_FUNCS} ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS})
add_executable(square_to_exact_cover greeksq_to_exc.c ${TYPES_AND_FUNCS} ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(exact_cover_to_square exact_cover_to_square.c ${TYPES_AND_FUNCS} ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(orthogonal_by_exact_cover greeklatinsq_by_exc.c ${TYPES_AND_FUNCS} ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(generator generator.c ${TYPES_AND_FUNCS} ${VECTOR} ${USER_INTERACTIONS} ${GREEKLATINSQ} ${EXACT_COVER})
add_executable(sudoku sudoku.c)

target_link_libraries(greeklatinsquares m)
target_link_libraries(exact_cover_solv m)
target_link_libraries(fib_lvl_2 m)
target_link_libraries(square_to_exact_cover m)
target_link_libraries(exact_cover_to_square m)
target_link_libraries(orthogonal_by_exact_cover m)
target_link_libraries(generator m)
target_link_libraries(sudoku m)

include_directories(PUBLIC gen_lib/)