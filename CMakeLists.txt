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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")
find_package(Allegro REQUIRED)
include_directories(${ALLEGRO_INCLUDE_DIR})
find_package(Freetype REQUIRED)
set(WZ_ALLEGRO_LDFLAGS ${ALLEGRO_LIBRARIES} ${FREETYPE_LIBRARIES})

if(WIN32)
	set(EXECUTABLE_TYPE "WIN32")
endif(WIN32)

if(APPLE)
    set(WANT_X11 off)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
    if(IPHONE)
       set(WZ_ALLEGRO_LDFLAGS ${WZ_ALLEGRO_LDFLAGS}"-framework CoreGraphics -framework QuartzCore -framework UIKit -framework Foundation -framework CoreFoundation -framework OpenGLES -framework OpenAL -lm")
       set(SDKROOT "iphoneos")
       set(CMAKE_OSX_SYSROOT "${SDKROOT}")
       add_definitions(-DIPHONE)
    else(IPHONE)
       set(WZ_ALLEGRO_LDFLAGS ${WZ_ALLEGRO_LDFLAGS} "-framework Cocoa -framework QuartzCore -framework Foundation -framework CoreFoundation -framework OpenGL -framework IOKit -framework AudioToolbox -framework OpenAL -lm")  
       add_definitions(-DAPPLE)
    endif(IPHONE)
endif(APPLE)

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
