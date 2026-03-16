# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD waveshare_rp2350_pizero CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(06_Proyecto C CXX ASM) # Como se llama el programa y lenguajes usados

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(06_Proyecto 06_Proyecto.c ) # Crear ejecutable con el archivo .c

pico_set_program_name(06_Proyecto "06_Proyecto")
pico_set_program_version(06_Proyecto "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(06_Proyecto 0)
pico_enable_stdio_usb(06_Proyecto 0)

add_subdirectory(libdvi)

if(NOT DEFINED DVI_DEFAULT_SERIAL_CONFIG)

	if(PICO_BOARD STREQUAL "waveshare_rp2350_pizero")
		set(DVI_DEFAULT_SERIAL_CONFIG "waveshare_rp2350_pizero" CACHE STRING
			"Select a default pin configuration from common_dvi_pin_config.h")
	else()
		set(DVI_DEFAULT_SERIAL_CONFIG "pico_sock_cfg" CACHE STRING
			"Select a default pin configuration from common_dvi_pin_configs.h")
	endif()

endif()

target_compile_definitions(06_Proyecto PRIVATE
    DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
)

if(PICO_RP2350)
    target_compile_definitions(06_Proyecto PRIVATE
        PICO_PIO_USE_GPIO_BASE=1
    )
endif()

# Add the standard library to the build
target_link_libraries(06_Proyecto
        pico_stdlib
        pico_multicore
        pico_util
        libdvi
)

# Add the standard include files to the build
target_include_directories(06_Proyecto PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_add_extra_outputs(06_Proyecto)

