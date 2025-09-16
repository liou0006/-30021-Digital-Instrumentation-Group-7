cmake_minimum_required(VERSION 3.20)
project(stm32_f303 C CXX ASM)

# --- Cross toolchain ---
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(TOOLCHAIN arm-none-eabi)

set(CMAKE_C_COMPILER   ${TOOLCHAIN}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}-gcc)

# --- MCU flags (F3, Cortex-M4F) ---
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# --- Common compile flags ---
add_compile_options(
        ${MCU_FLAGS}
        -O2 -ffunction-sections -fdata-sections
        -Wall -Wextra
)

# --- Definitions used by SPL ---
add_definitions(-DSTM32F30X -DUSE_STDPERIPH_DRIVER)

# --- Include paths ---
include_directories(
        ${CMAKE_SOURCE_DIR}/Inc
        ${CMAKE_SOURCE_DIR}/SPL/inc
)

# --- Sources ---
file(GLOB APP_SRC
        ${CMAKE_SOURCE_DIR}/Src/*.c
        ${CMAKE_SOURCE_DIR}/SPL/src/*.c
)

# If you have duplicates, explicitly list files instead of GLOB.

add_executable(${PROJECT_NAME}.elf ${APP_SRC})

# --- Linker script (edit path/name to yours) ---
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/SPL/ldscripts/STM32F303XC_FLASH.ld")
target_link_options(${PROJECT_NAME}.elf PRIVATE
        ${MCU_FLAGS}
        -Wl,--gc-sections
        -T ${LINKER_SCRIPT}
        -Wl,-Map=${PROJECT_NAME}.map
)

# Optional binary/hex
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${TOOLCHAIN}-objcopy -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
        COMMAND ${TOOLCHAIN}-objcopy -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
)
