# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

# https://cmake.org/cmake/help/latest/command/add_executable.html#command:add_executable
add_executable(christmas_snowflakes
	main.c
)

target_compile_options(christmas_snowflakes PRIVATE -Wall)

target_compile_definitions(christmas_snowflakes PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	)

if(PICO_RP2350)
    target_compile_definitions(christmas_snowflakes PRIVATE
        PICO_PIO_USE_GPIO_BASE=1
    )
endif()

target_link_libraries(christmas_snowflakes
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	libsprite
)

# create map/bin/hex file etc.
pico_add_extra_outputs(christmas_snowflakes)