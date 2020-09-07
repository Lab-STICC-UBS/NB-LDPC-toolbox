cmake_minimum_required (VERSION 3.13.4) # setting this is required

message ("CMake build type is \"" ${CMAKE_BUILD_TYPE} "\"")

set (CMAKE_C_COMPILER gcc)
set (CMAKE_C_STANDARD 11)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

set (CMAKE_C_FLAGS_DEBUG "-O0 -ggdb -DDEBUG=2")
set (CMAKE_C_FLAGS_RELEASE "-O2 -D_FORTIFY_SOURCE=2")

project (NB-LDPC-C-Toolbox LANGUAGES C) # this sets the project name

# ##############################################################################
# file globbing ##############################################################
# ##############################################################################

# These instructions search the directory tree when cmake is invoked and put all
# files that match the pattern in the variables `sources` and `data`.
file (GLOB source-files sources/nb_ldpc_toolbox.c sources/nb_ldpc_toolbox.h
		sources/struct.h)

include_directories (sources)

# ##############################################################################
# target definitions #########################################################
# ##############################################################################

add_compile_options (-Wall -pedantic -Wextra -Werror)

add_link_options (-lm)

add_library (nb_ldpc_toolbox ${source-files})

add_library (nb_ldpc_toolbox_dyn SHARED ${source-files})

add_executable (file-generator sources/main.c $<TARGET_OBJECTS:nb_ldpc_toolbox>)
