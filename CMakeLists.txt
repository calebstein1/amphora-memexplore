cmake_minimum_required(VERSION 3.21)

project(amphora_memory_explorer LANGUAGES C)
set(CMAKE_C_STANDARD 90)

string(APPEND CMAKE_C_FLAGS_DEBUG " -DDEBUG -fsanitize=address,undefined -Werror=all -Werror=pedantic -Wextra")

add_executable(memxplore main.c
	heap_handler.c)
