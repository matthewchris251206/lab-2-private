# CMake version requirements
cmake_minimum_required(VERSION 3.21...3.28)
if(${CMAKE_VERSION} VERSION_LESS 3.28)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION} ${CMAKE_MINOR_VERSION})
endif()

# project information
project(BaseRuntime VERSION 0.1.0
        DESCRIPTION "Conerstone of porting newlib into our own hardware platform"
        LANGUAGES C ASM)
set(CMAKE_TOOLCHAIN_FILE ${PROJECT_SOURCE_DIR}/cmake/rv32i.cmake)

# set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# include src and test-progs subdirectories
add_subdirectory(src)
add_subdirectory(test-progs)
add_subdirectory(asm-prog-assignment)
add_subdirectory(iss)

# Testing
if (PROJECT_IS_TOP_LEVEL)
  include(CTest)
  if (BUILD_TESTING)
    add_subdirectory(tests)
  endif()
endif()
