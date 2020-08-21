cmake_minimum_required (VERSION 3.13.4) # setting this is required

SET (CMAKE_C_COMPILER   gcc)
SET (CMAKE_C_STANDARD   11)
SET (CMAKE_CXX_COMPILER g++)
SET (CMAKE_CXX_STANDARD 14)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

project(NB-LDPC-Toolbox)            # this sets the project name

###############################################################################
## file globbing ##############################################################
###############################################################################

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`.
file (GLOB source-files  sources/nb_ldpc_toolbox.c sources/nb_ldpc_toolbox.h sources/struct.h)

include_directories (sources)

###############################################################################
## target definitions #########################################################
###############################################################################

add_compile_options (-Wall -pedantic -Wextra -Werror)

add_link_options (-lm)

add_library (nb_ldpc_toolbox ${source-files})

add_library (nb_ldpc_toolbox_dyn SHARED ${source-files})

add_executable (file-generator sources/main.c $<TARGET_OBJECTS:nb_ldpc_toolbox>)
