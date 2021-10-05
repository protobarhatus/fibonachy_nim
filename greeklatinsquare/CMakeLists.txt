cmake_minimum_required(VERSION 3.16)
project(greeklatingsq C)

set(CMAKE_C_STANDARD 99)

set(SRCS arrayint.c Array2dInt.c permutations.c greeklatinsquare.c user_interactions.c)

add_executable(greeklatingsq main.c ${SRCS})
add_executable(generator generator.c ${SRCS})

target_link_libraries(greeklatingsq m)
target_link_libraries(generator m)