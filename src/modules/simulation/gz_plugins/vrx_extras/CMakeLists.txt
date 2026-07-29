project(VrxExtras)


if(NOT DEFINED ENV{GZ_DISTRO} OR NOT "$ENV{GZ_DISTRO}" STREQUAL "harmonic")
    find_package(gz-math NAMES gz-math8 gz-math7 REQUIRED)
    find_package(gz-common NAMES gz-common6 gz-common5 REQUIRED)
    find_package(gz-msgs NAMES gz-msgs11 gz-msgs10 REQUIRED)
    find_package(gz-rendering NAMES gz-rendering9 gz-rendering8 REQUIRED)
else()
    find_package(gz-math NAMES gz-math7 REQUIRED)
    find_package(gz-common NAMES gz-common5 REQUIRED)
    find_package(gz-msgs NAMES gz-msgs10 REQUIRED)
    find_package(gz-rendering NAMES gz-rendering8 REQUIRED)
endif()
find_package(Eigen3 REQUIRED)

add_library(PolyhedraBuoyancyDrag SHARED
    PolyhedraBuoyancyDrag.cc
    PolyhedronVolume.cc
    ShapeVolume.cc
    Wavefield.cc
)

target_link_libraries(PolyhedraBuoyancyDrag
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-math${gz-math_VERSION_MAJOR}::gz-math${gz-math_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
    PUBLIC Eigen3::Eigen
)

target_include_directories(PolyhedraBuoyancyDrag
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(PolyhedraBuoyancyDrag PRIVATE -w)

add_library(USVWind SHARED
    USVWind.cc
)

target_link_libraries(USVWind
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-math${gz-math_VERSION_MAJOR}::gz-math${gz-math_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
    PUBLIC Eigen3::Eigen
)

target_include_directories(USVWind
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(USVWind PRIVATE -w)

add_library(PublisherPlugin SHARED
    PublisherPlugin.cc
)

target_link_libraries(PublisherPlugin
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
)

target_include_directories(PublisherPlugin
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(PublisherPlugin PRIVATE -w)

add_library(WaveVisual SHARED
    WaveVisual.cc
    Wavefield.cc
)

target_link_libraries(WaveVisual
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-math${gz-math_VERSION_MAJOR}::gz-math${gz-math_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
    PUBLIC gz-rendering${gz-rendering_VERSION_MAJOR}::gz-rendering${gz-rendering_VERSION_MAJOR}
)

target_include_directories(WaveVisual
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(WaveVisual PRIVATE -w)
