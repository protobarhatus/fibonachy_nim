cmake_minimum_required(VERSION 3.16)
project(fibonachy_nim C)

set(CMAKE_C_STANDARD 99)

include(exact_cover/CMakeLists.txt)
include(gen_lib/vector/CMakeLists.txt)
include(greeklatinsquare/CMakeLists.txt)
include(gen_lib/user_interacts/CMakeLists.txt)

#add_subdirectory(exact_cover)
#add_subdirectory (greeklatinsquare)
#add_subdirectory (vector)
#add_subdirectory (user_interacts)


add_executable(fib_lvl_1 fib\ lvl1.c)
add_executable(fib_lvl_2 fib\ lvl\ 2.c)
add_executable(greeklatinsquares greeklatinsq.c ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(exact_cover_solv exact_cover_main.c  ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(square_to_exact_cover greeksq_to_exc.c ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(exact_cover_to_square exact_cover_to_square.c ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})
add_executable(orthogonal_by_exact_cover greeklatinsq_by_exc.c ${VECTOR} ${EXACT_COVER} ${USER_INTERACTIONS} ${GREEKLATINSQ})

target_link_libraries(greeklatinsquares m)
target_link_libraries(exact_cover_solv m)
target_link_libraries(fib_lvl_2 m)
target_link_libraries(square_to_exact_cover m)
target_link_libraries(exact_cover_to_square m)
target_link_libraries(orthogonal_by_exact_cover m)