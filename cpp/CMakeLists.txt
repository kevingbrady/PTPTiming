cmake_minimum_required (VERSION 3.1)
ENABLE_LANGUAGE(CXX)

project (PTPMonitor)

set(CMAKE_C_STANDARD  11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_custom_target(inotify
                  COMMAND make
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/inotify
        )

include_directories(${PROJECT_SOURCE_DIR}/inotify)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/StatusLogWatcher.cpp
    ${PROJECT_SOURCE_DIR}/StatusLogWatcher.h

)

add_executable(ptp_monitor main.cpp ${SOURCES})
set_target_properties(ptp_monitor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ptp_monitor ${PROJECT_SOURCE_DIR}/inotify_cpp.a)
add_dependencies(ptp_monitor inotify)
target_link_libraries(ptp_monitor pthread)