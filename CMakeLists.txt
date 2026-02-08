# This version of cmake, and this branch, is optimized for codespaces and cmake v3.2
# It may not work with environments set up for the other branches.

cmake_minimum_required(VERSION 3.2)
project(adevs_irp)

set(CMAKE_CXX_STANDARD 17)

# Adjust if your adevs-3.3 is elsewhere
set(ADEVS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/adevs-3.3")


# ----------------------------
# Library target: IrpModel
# ----------------------------
add_library(IrpModel STATIC
        src/irpmodel/Retailer.cpp
)

include_directories(
        "${ADEVS_DIR}/include"
)

target_include_directories(IrpModel PUBLIC
        "src/irp-iso/irpdomain"
        "${DEVS_SF_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/irpmodel"
)

if (MSVC)
    target_compile_options(IrpModel PRIVATE /W4)
else()
    target_compile_options(IrpModel PRIVATE -Wall -Wextra -pedantic)
endif()

# ----------------------------
# Executable (tutorial runner)
# ----------------------------
add_executable(adevs_irp src/irpmodeltest/main.cpp
        src/irp-iso/irpdomain/Delivery.h
        src/irp-iso/irpdomain/InventoryCost.h
        src/irp-iso/irpdomain/Coordinate.h
        src/irpmodel/Retailer.cpp
        src/irp-iso/irpdomain/FacilityProps.h
        src/irpmodeltest/ReceiverTestGenerator.cpp
        src/irpmodeltest/RetailerTestAcceptor.cpp
)

target_include_directories(adevs_irp PRIVATE
        "src/irp-iso/irpdomain"
        "${DEVS_SF_DIR}/include"
)


if (MSVC)
    target_compile_options(adevs_irp PRIVATE /W4)
else()
    target_compile_options(adevs_irp PRIVATE -Wall -Wextra -pedantic)
endif()


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
