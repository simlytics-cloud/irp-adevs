cmake_minimum_required(VERSION 4.1)
project(adevs_irp)

set(CMAKE_CXX_STANDARD 17)

# Adjust if your adevs-3.3 is elsewhere
set(ADEVS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../adevs-3.3")
set(DEVS_SF_DIR "/Users/robertkewley/Library/CloudStorage/OneDrive-simlytics.cloud/Simlytics/dev/simlytics/devs/cpp/devs-sf-cpp/_install")

# Default "local install" inside the tutorial folder (can still be overridden by user)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/_install" CACHE PATH "" FORCE)
endif()

find_package(nlohmann_json REQUIRED)

# ----------------------------
# Library target: IrpModel
# ----------------------------
add_library(IrpModel STATIC
        src/irpmodel/Retailer.cpp
        src/irpmodel/Manufacturer.cpp
        src/irpmodel/Manufacturer.h
        src/irpmodel/Vehicle.cpp
        src/irpmodel/Vehicle.h
        src/irp-iso/irpdomain/IrpEvent.cpp
        src/irpmodel/IrpData.h
)

include_directories(
        "${ADEVS_DIR}/include"
)

target_include_directories(IrpModel PUBLIC
        "src/irp-iso/irpdomain"
        "${DEVS_SF_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/irpmodel"
)
target_link_libraries(IrpModel PUBLIC nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(IrpModel PRIVATE /W4)
else()
    target_compile_options(IrpModel PRIVATE -Wall -Wextra -pedantic)
endif()

add_subdirectory("src/irp-iso")
add_subdirectory("src/irpmodeltest")
# ----------------------------
include(GNUInstallDirs)

# Install the reusable library
install(TARGETS IrpModel
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install domain headers
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install model headers (Retailer.h, etc.) under include/irpmodel
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/irpmodel/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/irpmodel
        FILES_MATCHING PATTERN "*.h"
)
