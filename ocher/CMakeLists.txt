include_directories(
	${ROOT_DIR}
	${ZLIB_INCLUDE_DIR}
	${THIRD_PARTY_DIR}
	.
	)
if(OCHER_KOBO)
	include_directories(${CMAKE_SOURCE_DIR}/linux)
endif()

list(APPEND OCHER_SOURCES
	Container.cpp
	device/Battery.cpp
	device/Device.cpp
	device/Filesystem.cpp
	fmt/Format.cpp
	fmt/Layout.cpp
	fmt/minizip/ioapi.c
	fmt/minizip/unzip.c
	#resources/Bitmaps.cpp
	settings/Settings.cpp
	shelf/Meta.cpp
	shelf/Shelf.cpp
	util/Bitops.h
	util/Buffer.cpp
	util/Buffer.h
	util/Debug.cpp
	util/Debug.h
	util/DirIter.cpp
	util/DirIter.h
	util/File.cpp
	util/File.h
	util/Intrinsics.h
	util/Monitor.cpp
	util/Monitor.h
	util/MurmurHash2.cpp
	util/MurmurHash2.h
	util/Path.cpp
	util/Path.h
	util/StrUtil.cpp
	util/StrUtil.h
	ux/Activity.cpp
	ux/BatteryIcon.cpp
	#ux/BootActivityFb.cpp
	ux/ClockIcon.cpp
	ux/Controller.cpp
	#ux/HomeActivityFb.cpp
	#ux/LibraryActivityFb.cpp
	ux/MainWindow.cpp
	ux/NavBar.cpp
	ux/Pagination.cpp
	ux/PowerSaver.cpp
	#ux/ReadActivityFb.cpp
	#ux/SettingsActivityFb.cpp
	#ux/SleepActivityFb.cpp
	#ux/SyncActivityFb.cpp
	ux/SystemBar.cpp
	)
if(OCHER_KOBO)
	list(APPEND OCHER_SOURCES
		ux/mx50/FrameBufferMx50.cpp
		device/kobo/KoboEvents.cpp
		)
	list(APPEND OCHER_DEFS UX_FB UX_FB_MX50)
endif()

if(OCHER_FMT_EPUB)
	list(APPEND OCHER_SOURCES
		fmt/epub/LayoutEpub.cpp
		fmt/epub/UnzipCache.cpp
		fmt/epub/Epub.cpp
		)
	list(APPEND OCHER_DEFS OCHER_EPUB)
endif()
if(OCHER_FMT_TEXT)
	list(APPEND OCHER_SOURCES
		fmt/text/Text.cpp
		fmt/text/LayoutText.cpp
		)
	list(APPEND OCHER_DEFS OCHER_TEXT)
endif()
if(OCHER_FMT_JPEG)
	list(APPEND OCHER_DEFS OCHER_JPEG)
endif()

list(APPEND OCHER_HEADERS
	device/Filesystem.h
	ux/Controller.h
	ux/MainWindow.h
	ux/PowerSaver.h
	)

QT4_WRAP_CPP(OCHER_MOC_SOURCES ${OCHER_HEADERS})
add_library(ocherbook STATIC
	${OCHER_SOURCES}
	${OCHER_MOC_SOURCES}
	)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND OCHER_DEFS LOG_LEVEL=5)
else()
	list(APPEND OCHER_DEFS LOG_LEVEL=2)
endif()

target_compile_definitions(ocherbook
	PRIVATE
		${OCHER_DEFS}
	)

target_link_libraries(ocherbook
	zlib
	mxml
	${QT_LIBRARIES}
	)

add_executable(ocher
	ocher.cpp
	)
target_link_libraries(ocher
	ocherbook
	)


include(ProcessorCount)
ProcessorCount(NUM_CPUS)
if(NOT NUM_CPUS EQUAL 0)
	set(CPPCHECK_CPUS -j${NUM_CPUS})
endif()
get_target_property(OCHER_INCS ocherbook INCLUDE_DIRECTORIES)
foreach(INC ${OCHER_INCS})
	list(APPEND CPPCHECK_INCS "-I${INC}")
endforeach()
add_custom_target(cppcheck
	COMMAND cppcheck
		--max-configs=100 ${CPPCHECK_CPUS}
		-i${THIRD_PARTY_DIR} --config-exclude=${THIRD_PARTY_DIR}
		--std=c++11 --std=posix --language=c++
		--enable=warning
		${CPPCHECK_INCS}
		${CMAKE_CURRENT_SOURCE_DIR} 2> ${CMAKE_CURRENT_BINARY_DIR}/cppcheck.log
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)


configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile
	${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

add_custom_target(doxygen
	COMMAND doxygen ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)

add_subdirectory(test)
