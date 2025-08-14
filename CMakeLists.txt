cmake_minimum_required(VERSION 3.21)

project(amphora_memory_explorer LANGUAGES C)

add_executable(memexplore main.c
	commands.c)
