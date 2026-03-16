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

project(07_Proyecto_pico_pio_usb C CXX ASM)
set(PICO_TINYUSB_HOST_ENABLED 1)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Clonar Pico-PIO-USB al preoyecto:
# git clone "https://github.com/sekigon-gonnoc/Pico-PIO-USB"
# Indicar a CMake que tenemos este nuevo directorio, luego en agregar pio_usb al resto de librerias existentes target_link_libraries(pico_pio_usb) 
add_subdirectory(Pico-PIO-USB)

# Add executable. Default name is the project name, version 0.1

add_executable(07_Proyecto_pico_pio_usb
    07_Proyecto_pico_pio_usb.c
)

target_compile_definitions(07_Proyecto_pico_pio_usb PUBLIC
    CFG_TUH_RPI_PIO_USB=1
    PICO_DEFAULT_PIO_USB_DP_PIN=28
)
target_compile_definitions(07_Proyecto_pico_pio_usb PRIVATE
    BOARD_TUH_RHPORT=1
)

pico_generate_pio_header(07_Proyecto_pico_pio_usb
    ${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB/src/usb_tx.pio
)

pico_generate_pio_header(07_Proyecto_pico_pio_usb
    ${CMAKE_CURRENT_LIST_DIR}/Pico-PIO-USB/src/usb_rx.pio
)
pico_set_program_name(07_Proyecto_pico_pio_usb "07_Proyecto_pico_pio_usb")
pico_set_program_version(07_Proyecto_pico_pio_usb "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(07_Proyecto_pico_pio_usb 1)
pico_enable_stdio_usb(07_Proyecto_pico_pio_usb 0)

target_sources(07_Proyecto_pico_pio_usb PRIVATE
    Pico-PIO-USB/src/pio_usb.c
    Pico-PIO-USB/src/pio_usb_device.c
    Pico-PIO-USB/src/pio_usb_host.c
    Pico-PIO-USB/src/usb_crc.c
)

# Add the standard library to the build
target_link_libraries(07_Proyecto_pico_pio_usb
        pico_stdlib
        hardware_pio
        hardware_dma
        pico_multicore
        tinyusb_host
        tinyusb_board
        pico_pio_usb
        )

# Add the standard include files to the build
target_include_directories(07_Proyecto_pico_pio_usb PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${PICO_SDK_PATH}/lib/tinyusb/src
)

pico_add_extra_outputs(07_Proyecto_pico_pio_usb)

