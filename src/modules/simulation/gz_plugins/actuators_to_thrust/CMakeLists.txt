project(ActuatorsToThrust)

if(NOT DEFINED ENV{GZ_DISTRO} OR NOT "$ENV{GZ_DISTRO}" STREQUAL "harmonic")
    find_package(gz-msgs NAMES gz-msgs11 gz-msgs10 REQUIRED)
else()
    find_package(gz-msgs NAMES gz-msgs10 REQUIRED)
endif()

add_library(${PROJECT_NAME} SHARED
    ActuatorsToThrust.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
