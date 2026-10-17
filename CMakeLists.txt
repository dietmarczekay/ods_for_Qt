# Copyright (c) 2026 dczekay@gmx.de
# SPDX-License-Identifier: GPL-3.0-or-later
#
# This file is part of QODS_Module, licensed under the GNU General Public
# License v3 or later. See COPYING or <https://www.gnu.org/licenses/>.

# ODS writer (QODS_Module). Needs Qt6 Core/Gui and zlib (no QuaZip).
find_package(Qt6 REQUIRED COMPONENTS Core Gui)

file(GLOB ODS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB ODS_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(ods_library STATIC ${ODS_SOURCES} ${ODS_HEADERS})
target_include_directories(ods_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ods_library PUBLIC Qt6::Core Qt6::Gui)

if(EMSCRIPTEN)
    # zlib comes with Emscripten as a port
    target_compile_options(ods_library PUBLIC -sUSE_ZLIB=1)
    target_link_options(ods_library PUBLIC -sUSE_ZLIB=1)
else()
    find_package(ZLIB REQUIRED)
    target_link_libraries(ods_library PRIVATE ZLIB::ZLIB)
endif()
