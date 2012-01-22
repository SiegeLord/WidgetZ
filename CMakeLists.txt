cmake_minimum_required(VERSION 2.8)
project(WIDGETZ C)

include_directories(
	${WIDGETZ_SOURCE_DIR}/include
)

file(GLOB_RECURSE SOURCES "src/*.c")
if(NOT USE_INTERNAL_LIB)
set(EXAMPLE_DATA "DejaVuSans.ttf")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")

include(FindPkgConfig)

if(WIN32)
	set(EXECUTABLE_TYPE "WIN32")
	set(WZ_ALLEGRO_LDFLAGS -lallegro_ttf -lallegro_font -lallegro -lallegro_primitives)
else(WIN32)
	pkg_check_modules(WZ_ALLEGRO REQUIRED allegro_primitives-5.0 allegro_ttf-5.0)
endif(WIN32)

add_custom_target(example_data ALL DEPENDS ${EXAMPLE_DATA})

foreach(file ${EXAMPLE_DATA})
	add_custom_command(
		OUTPUT ${file}
		COMMAND "${CMAKE_COMMAND}" -E copy
				"${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${file}"
		)
endforeach(file)
endif(NOT USE_INTERNAL_LIB)
add_library(widgetz ${SOURCES})
if(NOT USE_INTERNAL_LIB)
target_link_libraries(widgetz ${WZ_ALLEGRO_LDFLAGS})
add_executable(example "example/example.c")
target_link_libraries(example widgetz ${WZ_ALLEGRO_LDFLAGS})

install(DIRECTORY "${WIDGETZ_SOURCE_DIR}/include/widgetz" DESTINATION include)
install(TARGETS widgetz ARCHIVE DESTINATION lib)
else(NOT USE_INTERNAL_LIB)
target_link_libraries(widgetz ${ALLEGRO_LIBRARIES})
endif(NOT USE_INTERNAL_LIB)