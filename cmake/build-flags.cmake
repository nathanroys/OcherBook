set(_CFLAGS_WARN "-Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wformat")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${_CFLAGS_WARN}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_CFLAGS_WARN}")

set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -DNDEBUG")

if(NOT "${TOOLCHAIN_C_FLAGS}" STREQUAL "")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TOOLCHAIN_C_FLAGS}")
endif()
if(NOT "${TOOLCHAIN_CXX_FLAGS}" STREQUAL "")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TOOLCHAIN_CXX_FLAGS}")
endif()

if(NOT "$ENV{CFLAGS}" STREQUAL "")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} $ENV{CFLAGS}")
endif()
if(NOT "$ENV{CXXFLAGS}" STREQUAL "")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
endif()