# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

# https://cmake.org/cmake/help/latest/command/add_executable.html#command:add_executable
add_executable(hello_dvi
	main.c
)

# https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options
target_compile_options(hello_dvi PRIVATE -Wall)

# https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions
target_compile_definitions(hello_dvi PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	)
# https://cmake.org/cmake/help/latest/command/if.html#command:if
if(PICO_RP2350)
    target_compile_definitions(hello_dvi PRIVATE
        PICO_PIO_USE_GPIO_BASE=1
    )
endif()

# https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries
target_link_libraries(hello_dvi
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
)

# create map/bin/hex file etc.
# https://github.com/raspberrypi/pico-sdk/blob/master/src/cmake/on_device.cmake
pico_add_extra_outputs(hello_dvi)
