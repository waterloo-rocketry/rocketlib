# To include a module in parent project with CMake target named 'my_app':
#	add_subdirectory(third_party/rocketlib)
#	target_link_libraries(my_app PRIVATE rocketlib::can_message rocketlib::stm32_common) <- Add all directories where we need source files
# If no source files are needed, only headers, replace the target_link_libraries line:
#	target_link_libraries(my_app PRIVATE rocketlib::headers)
 
cmake_minimum_required(VERSION 3.22)

set(ROCKETLIB_ROOT ${CMAKE_CURRENT_LIST_DIR})

# Rocketlib headers included relative to root rocketlib/ directory. 
add_library(rocketlib_headers INTERFACE)
add_library(rocketlib::headers ALIAS rocketlib_headers)
target_include_directories(rocketlib_headers INTERFACE ${ROCKETLIB_ROOT})

# Function to define modules (top-level subdirectories containing source files) for parent app to include as CMake libraries.
# rocketlib_sources(<module_name> [SOURCE_FILES <source file names>] [DEPENDS <other dir names>...])
function(rocketlib_sources module_name)
	cmake_parse_arguments(ARG "" "" "SOURCE_FILES;DEPENDS" ${ARGN})

	add_library(rocketlib_${module_name} INTERFACE)
	add_library(rocketlib::${module_name} ALIAS rocketlib_${module_name})

	list(TRANSFORM ARG_SOURCE_FILES PREPEND ${ROCKETLIB_ROOT}/${module_name}/ OUTPUT_VARIABLE source_file_paths)
	target_sources(rocketlib_${module_name} INTERFACE ${source_file_paths})

	foreach(dep IN LISTS ARG_DEPENDS)
		target_link_libraries(rocketlib_${module_name} INTERFACE rocketlib::${dep})
	endforeach()

	# Rocketlib internal includes are relative to root rocketlib/ directory so if any sources are added we must also set the include dir.
	target_link_libraries(rocketlib_${module_name} INTERFACE rocketlib::headers)
endfunction()

# Register sources in each module as a library.
rocketlib_sources(can_buffer
	SOURCE_FILES can_rcv_buffer.c can_tx_buffer.c safe_ring_buffer.c
)
rocketlib_sources(can_message
	SOURCE_FILES msg_actuator.c msg_canards.c msg_general.c msg_gps.c msg_recovery.c msg_sensor.c msg_stream.c msg_telemetry.c
)
rocketlib_sources(common
	SOURCE_FILES crc8.c low_pass_filter.c
)
rocketlib_sources(logging
	SOURCE_FILES mbr.c
)
rocketlib_sources(mcp2515
	SOURCE_FILES mcp2515.c
)
rocketlib_sources(pic18f26k83
	SOURCE_FILES pic18f26k83_can.c pic18f26k83_can_timing_util.c pic18f26k83_timer.c
)
rocketlib_sources(stm32_common
	SOURCE_FILES stm32_can.c
)

