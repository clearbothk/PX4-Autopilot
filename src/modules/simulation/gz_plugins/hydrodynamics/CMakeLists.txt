############################################################################
#
#   Copyright (c) 2025 PX4 Development Team. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name PX4 nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

# Hydrodynamics/buoyancy plugins for surface vehicles (boat/WAM-V hull),
# vendored from VRX (osrf/vrx, Apache License 2.0) to remove the runtime
# dependency on an external VRX checkout for the wamv gz model.
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

# Vendored third-party source (osrf/vrx) -- don't hold it to PX4's own
# strict warning/-Werror bar, we're not maintaining this code upstream.
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

# Vendored third-party source (osrf/vrx) -- don't hold it to PX4's own
# strict warning/-Werror bar, we're not maintaining this code upstream.
target_compile_options(Surface PRIVATE -w)
