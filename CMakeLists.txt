cmake_minimum_required(VERSION 3.8)
project(stemmer)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c learning.c word_processing.c word.h learning.h word_processing.h)
add_executable(stemmer ${SOURCE_FILES})