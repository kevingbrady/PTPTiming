cmake_minimum_required (VERSION 3.1)
ENABLE_LANGUAGE(CXX)

project (PTPSensor)

set(CMAKE_C_STANDARD  11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_custom_target(inotify
                  COMMAND make
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/inotify
        )

include_directories(${PROJECT_SOURCE_DIR}/inotify)

add_library(PTPLogWatcher
        ${PROJECT_SOURCE_DIR}/StatusLogWatcher.cpp
        ${PROJECT_SOURCE_DIR}/StatusLogWatcher.h
        )

add_library(PTPSensor
    ${PROJECT_SOURCE_DIR}/PTPSensor.cpp
    ${PROJECT_SOURCE_DIR}/PTPSensor.h

)

add_executable(ptp_sensor main.cpp)
set_target_properties(ptp_sensor PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(ptp_sensor PTPSensor)
add_dependencies(PTPSensor PTPLogWatcher)
add_dependencies(PTPLogWatcher inotify)
target_link_libraries(ptp_sensor PTPSensor)
target_link_libraries(PTPSensor PTPLogWatcher)
target_link_libraries(PTPLogWatcher ${PROJECT_SOURCE_DIR}/inotify_cpp.a)
target_link_libraries(PTPLogWatcher pthread)
