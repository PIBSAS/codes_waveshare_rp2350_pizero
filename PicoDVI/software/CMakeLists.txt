# https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html#command:cmake_minimum_required
cmake_minimum_required(VERSION 3.13)

# https://cmake.org/cmake/help/latest/command/set.html#command:set
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Extension

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
# 
set(PICO_BOARD waveshare_rp2350_pizero CACHE STRING "Board type")

# https://cmake.org/cmake/help/latest/command/include.html#command:include
# Replace with the one on SDK installed
include(pico_sdk_import.cmake)

# https://cmake.org/cmake/help/latest/command/project.html#command:project
project(picodvi C CXX ASM)

# https://github.com/raspberrypi/pico-sdk/blob/master/external/pico_sdk_import.cmake
pico_sdk_init()

# https://cmake.org/cmake/help/latest/command/add_compile_options.html#command:add_compile_options
add_compile_options(-Wall)

# To change the default serial config for all apps, pass e.g.
#   cmake -DDVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg ..
# then rebuild. This is applied via target_compile_definitions on each app
# individually.

# https://cmake.org/cmake/help/latest/command/set.html#command:set
# https://cmake.org/cmake/help/latest/command/if.html#command:if
if(NOT DEFINED DVI_DEFAULT_SERIAL_CONFIG)

	if(PICO_BOARD STREQUAL "waveshare_rp2350_pizero")
		set(DVI_DEFAULT_SERIAL_CONFIG "waveshare_rp2350_pizero" CACHE STRING
			"Select a default pin configuration from common_dvi_pin_config.h")
	else()
		set(DVI_DEFAULT_SERIAL_CONFIG "pico_sock_cfg" CACHE STRING
			"Select a default pin configuration from common_dvi_pin_configs.h")
	endif()

endif()

# https://cmake.org/cmake/help/latest/command/include_directories.html#command:include_directories
include_directories(
	assets
	include
	)

# https://cmake.org/cmake/help/latest/command/add_subdirectory.html#command:add_subdirectory
add_subdirectory(libdvi)
add_subdirectory(libsprite)
add_subdirectory(apps)