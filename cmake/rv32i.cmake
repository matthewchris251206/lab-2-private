# Try to find riscv32-unknoen-elf-gcc executable
# find_file(RISCV_GNU_TOOLCHAIN_GCC_COMPILER "riscv32-unknown-elf-gcc" HINTS ENV PATH)
# if (EXISTS ${RISCV_GNU_TOOLCHAIN_GCC_COMPILER})
#   set(RISCV_GCC_COMPILER ${RISCV_GNU_TOOLCHAIN_GCC_COMPILER})
# else()
#   message(FATAL_ERROR "RISC-V cross-compiler is not found. ${RISCV_GNU_TOOLCHAIN_GCC_COMPILER}")
# endif()

# message("RISC-V GCC is found: ${RISCV_GCC_COMPILER}")
# get_filename_component(RISCV_TOOLCHAIN_BIN_PATH ${RISCV_GCC_COMPILER} DIRECTORY)
# get_filename_component(RISCV_TOOLCHAIN_BIN_GCC ${RISCV_GCC_COMPILER} NAME_WE)

# message( "RISC-V GCC Path: ${RISCV_TOOLCHAIN_BIN_PATH}" )
# STRING(REGEX REPLACE "\-gcc" "-" CROSS_COMPILE ${RISCV_TOOLCHAIN_BIN_GCC})
# message( "RISC-V Cross Compile: ${CROSS_COMPILE}" )

set(CROSS_COMPILE riscv32-unknown-elf-)

# basic information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR rv32i)
# set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# set compiler by  riscv-gnu-toolchain
set(CMAKE_ASM_COMPILER {CROSS_COMPILE}gcc )
set(CMAKE_AR ${CROSS_COMPILE}ar)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)

# set common compile flags
set(CMAKE_C_FLAGS "-march=${CMAKE_SYSTEM_PROCESSOR} -mabi=ilp32 -ffreestanding -fno-builtin -Wall -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" )
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -march=${CMAKE_SYSTEM_PROCESSOR} -nostartfiles -nostdlib -static")
