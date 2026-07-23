project(WamvHydrodynamics)

if(NOT DEFINED ENV{GZ_DISTRO} OR NOT "$ENV{GZ_DISTRO}" STREQUAL "harmonic")
    find_package(gz-math NAMES gz-math8 gz-math7 REQUIRED)
    find_package(gz-common NAMES gz-common6 gz-common5 REQUIRED)
    find_package(gz-msgs NAMES gz-msgs11 gz-msgs10 REQUIRED)
else()
    find_package(gz-math NAMES gz-math7 REQUIRED)
    find_package(gz-common NAMES gz-common5 REQUIRED)
    find_package(gz-msgs NAMES gz-msgs10 REQUIRED)
endif()
find_package(Eigen3 REQUIRED)

# SimpleHydrodynamics: added-mass/drag forces, no wave-field dependency
add_library(SimpleHydrodynamics SHARED
    SimpleHydrodynamics.cc
)

target_link_libraries(SimpleHydrodynamics
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-math${gz-math_VERSION_MAJOR}::gz-math${gz-math_VERSION_MAJOR}
    PUBLIC Eigen3::Eigen
)

target_include_directories(SimpleHydrodynamics
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(SimpleHydrodynamics PRIVATE -w)

# Surface: buoyancy relative to wave elevation (depends on Wavefield)
add_library(Surface SHARED
    Surface.cc
    Wavefield.cc
)

target_link_libraries(Surface
    PUBLIC gz-plugin${gz-plugin_VERSION_MAJOR}::gz-plugin${gz-plugin_VERSION_MAJOR}
    PUBLIC gz-sim${gz-sim_VERSION_MAJOR}::gz-sim${gz-sim_VERSION_MAJOR}
    PUBLIC gz-transport${gz-transport_VERSION_MAJOR}::gz-transport${gz-transport_VERSION_MAJOR}
    PUBLIC gz-common${gz-common_VERSION_MAJOR}::gz-common${gz-common_VERSION_MAJOR}
    PUBLIC gz-math${gz-math_VERSION_MAJOR}::gz-math${gz-math_VERSION_MAJOR}
    PUBLIC gz-msgs${gz-msgs_VERSION_MAJOR}::gz-msgs${gz-msgs_VERSION_MAJOR}
    PUBLIC Eigen3::Eigen
)

target_include_directories(Surface
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(Surface PRIVATE -w)
