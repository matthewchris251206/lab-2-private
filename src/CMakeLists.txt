include(${PROJECT_SOURCE_DIR}/cmake/rv32i.cmake)

# set source file list of libbase
set(LIB_SRCS basic.c print.c memory.c)

# setup target base (static library)
add_library(base STATIC)
target_sources(base PRIVATE ${LIB_SRCS})
target_include_directories(base PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(base PRIVATE ${COMMON_COMPILE_FLAG})

# setup startup files object (which defines _start symbol)
add_library(startup OBJECT ${CMAKE_SOURCE_DIR}/src/start.S)
target_compile_options(startup PRIVATE ${COMMON_COMPILE_FLAG})
