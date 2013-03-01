project(corelib)
cmake_minimum_required(VERSION 2.8)

include_directories(src)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -std=gnu99 -export-dynamic -fPIC")
set(BUILD_SHARED_LIBS true)

add_library(chained src/support/support.c src/support/strings.c src/support/cores.c src/data-structures/persistent_map.c src/data-structures/map.c src/data-structures/linklist.c src/data-structures/fifo.c src/data-structures/buffer.c src/main.c src/externs.c src/events.c src/timers.c 
)
target_link_libraries(chained ev dl)
