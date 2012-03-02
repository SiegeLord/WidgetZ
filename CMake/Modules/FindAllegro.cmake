# - Try to find Allegro 5.0
# Once done, this will define
# ALLEGRO_INCLUDE_DIR - allegro.h etc.
# ALLEGRO_LIBRARIES - for linking
# ALLEGRO_FOUND - True if found
# ALLEGRO_DEFINITIONS - compiler switches used for Allegro

set(ALLEGRO_NAMES allegro allegro_acodec allegro_audio allegro allegro_color allegro_font allegro_image allegro_memfile allegro_primitives allegro_ttf allegro_shader)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	list(APPEND ALLEGRO_NAMES allegro_main)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(ALLEGRO_SUFFIX "-5.1")
set(ALLEGRO_VERSION "-5.1.0")
set(ALLEGRO_WIN32_FLAVOR "-mt")

# use pkg-config
include(FindPkgConfig)
find_package(PkgConfig)
if(NOT APPLE)
if(PKG_CONFIG_FOUND)
foreach(ALLEGRO_SPECIFIC_LIB ${ALLEGRO_NAMES})
	pkg_check_modules(PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB} REQUIRED ${ALLEGRO_SPECIFIC_LIB}${ALLEGRO_SUFFIX})
	list(APPEND ALLEGRO_DEFINITIONS ${PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_CFLAGS_OTHER})
	string(COMPARE EQUAL "${PC_ALLEGRO_INCLUDE_DIRS}" "${PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_INCLUDE_DIRS}" INC_ALREADY_ADDED)
	string(COMPARE EQUAL "${PC_ALLEGRO_LIBRARY_DIRS}" "${PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_LIBRARY_DIRS}" LIB_ALREADY_ADDED)
	if(INC_ALREADY_ADDED)
	else(INC_ALREADY_ADDED)
		list(APPEND PC_ALLEGRO_INCLUDE_DIRS ${PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_INCLUDE_DIRS})
	endif(INC_ALREADY_ADDED)
	if(LIB_ALREADY_ADDED)
	else(LIB_ALREADY_ADDED)
		list(APPEND PC_ALLEGRO_LIBRARY_DIRS ${PC_ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_LIBRARY_DIRS})
	endif(LIB_ALREADY_ADDED)
endforeach(ALLEGRO_SPECIFIC_LIB)
endif(PKG_CONFIG_FOUND)
endif(NOT APPLE)

# use pkg-config returns as hints to find include directory
find_path(ALLEGRO_INCLUDE_DIRS allegro5/allegro.h
			HINTS ${PC_ALLEGRO_INCLUDEDIR} ${PC_ALLEGRO_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/Deps/include" "$ENV{ALLEGRO_HOME}/include" "$ENV{EXTDEV}/include" "$ENV{EPREFIX}/include" "$ENV{PREFIX}/include" "/usr/include" "/usr/local/include"
)

# use pkg-config returns as hints to find lib directory
foreach(ALLEGRO_SPECIFIC_LIB ${ALLEGRO_NAMES})
   find_library(ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_LIB
				NAMES ${ALLEGRO_SPECIFIC_LIB} ${ALLEGRO_SPECIFIC_LIB}${ALLEGRO_SUFFIX} ${ALLEGRO_SPECIFIC_LIB}${ALLEGRO_VERSION} ${ALLEGRO_SPECIFIC_LIB}${ALLEGRO_VERSION}${ALLEGRO_WIN32_FLAVOR} ${ALLEGRO_SPECIFIC_LIB}-static
				HINTS ${PC_ALLEGRO_LIBDIR} ${PC_ALLEGRO_LIBRARY_DIRS} "${PROJECT_SOURCE_DIR}/Deps/lib" "$ENV{ALLEGRO_HOME}/lib" "$ENV{EXTDEV}/lib" "$ENV{EXTDEV}/lib/MSVC" "$ENV{EXTDEV}/lib/MinGW" "$ENV{EXTDEV}/lib/BCC" "$ENV{EPREFIX}/lib" "$ENV{PREFIX}/lib" "/usr/lib" "/usr/local/lib"
   )
   list(APPEND ALLEGRO_LIBRARY ${ALLEGRO_${ALLEGRO_SPECIFIC_LIB}_LIB})
endforeach(ALLEGRO_SPECIFIC_LIB)

set(ALLEGRO_LIBRARIES "${ALLEGRO_LIBRARY}")
set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_INCLUDE_DIRS}")

include(FindPackageHandleStandardArgs)
# handle QUIETLY and REQUIRED args and set ALLEGRO_FOUND to TRUE if all listed variables are TRUE
find_package_handle_standard_args(Allegro DEFAULT_MSG ALLEGRO_LIBRARY ALLEGRO_INCLUDE_DIRS)

mark_as_advanced(ALLEGRO_INCLUDE_DIRS ALLEGRO_LIBRARY)
