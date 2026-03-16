# Note we are using INTERFACE so that the library can be configured per-app
# with compile-time defines

# https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library
add_library(libdvi INTERFACE)

# https://cmake.org/cmake/help/latest/command/target_sources.html#command:target_sources
target_sources(libdvi INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/dvi.c
	${CMAKE_CURRENT_LIST_DIR}/dvi.h
	${CMAKE_CURRENT_LIST_DIR}/dvi_config_defs.h
	${CMAKE_CURRENT_LIST_DIR}/dvi_serialiser.c
	${CMAKE_CURRENT_LIST_DIR}/dvi_serialiser.h
	${CMAKE_CURRENT_LIST_DIR}/dvi_timing.c
	${CMAKE_CURRENT_LIST_DIR}/dvi_timing.h
	${CMAKE_CURRENT_LIST_DIR}/tmds_encode.S
	${CMAKE_CURRENT_LIST_DIR}/tmds_encode.c
	${CMAKE_CURRENT_LIST_DIR}/tmds_encode.h
	${CMAKE_CURRENT_LIST_DIR}/tmds_table.h
	${CMAKE_CURRENT_LIST_DIR}/tmds_table_fullres.h
	${CMAKE_CURRENT_LIST_DIR}/util_queue_u32_inline.h
	)

# https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories
target_include_directories(libdvi INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries
target_link_libraries(libdvi INTERFACE
	pico_base_headers
	pico_util
	hardware_dma
	hardware_interp
	hardware_pio
	hardware_pwm
	)

pico_generate_pio_header(libdvi ${CMAKE_CURRENT_LIST_DIR}/dvi_serialiser.pio)
pico_generate_pio_header(libdvi ${CMAKE_CURRENT_LIST_DIR}/tmds_encode_1bpp.pio)