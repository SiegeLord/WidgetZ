cmake_minimum_required(VERSION 2.8)
project(WIDGETZ C)

set(WZ_SOURCES
	internal_utils.c
	theme.c
	utils.c
	widgets/box.c
	widgets/editbox.c
	widgets/scroll.c
	widgets/toggle.c
	widgets/button.c
	widgets/filllayout.c
	widgets/textbox.c
	widgets/widget.c
	widgets/layoutstop.c
	widgets/imagebutton.c
)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g")

include(FindPkgConfig)

if(WIN32)
	set(WZ_ALLEGRO_LDFLAGS -lallegro_ttf -lallegro_font -lallegro)
else(WIN32)
	pkg_check_modules(WZ_ALLEGRO REQUIRED allegro_primitives-5.0 allegro_ttf-5.0)
endif(WIN32)

add_library(widgetz ${WZ_SOURCES})
target_link_libraries(widgetz ${WZ_ALLEGRO_LDFLAGS})
