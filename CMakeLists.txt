cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(xy NONE)

# find ALPS Library
find_package(ALPS REQUIRED PATHS ${ALPS_ROOT_DIR} $ENV{ALPS_HOME} NO_SYSTEM_ENVIRONMENT_PATH)
message(STATUS "Found ALPS: ${ALPS_ROOT_DIR} (revision: ${ALPS_VERSION})")
include(${ALPS_USE_FILE})

# enable C and C++ compilers
enable_language(C CXX)

add_executable(xy xy.cpp)
target_link_libraries(xy ${ALPS_LIBRARIES})
add_alps_test(hello)
