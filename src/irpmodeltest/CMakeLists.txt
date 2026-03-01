# ----------------------------
# Retailer Test
# ----------------------------
add_executable(retailer_test retailer_test.cpp
        ../irp-iso/irpdomain/Delivery.h
        ../irp-iso/irpdomain/InventoryCost.h
        ../irp-iso/irpdomain/Coordinate.h
        ../irpmodel/Retailer.cpp
        ../irp-iso/irpdomain/FacilityProps.h
        ReceiverTestGenerator.cpp
        RetailerTestAcceptor.cpp
        ../irpmodel/IrpData.h)

target_include_directories(retailer_test PRIVATE
        "../irp-iso/irpdomain"
        "${DEVS_SF_DIR}/include"
)

target_compile_definitions(retailer_test PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../data")

target_link_libraries(retailer_test PUBLIC IrpModel nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(retailer_test PRIVATE /W4)
else()
    target_compile_options(retailer_test PRIVATE -Wall -Wextra -pedantic)
endif()

# ----------------------------
# Manufacturer Test
# ----------------------------
add_executable(manufacturer_test 
        manufacturer_test.cpp
        ManufacturerTestGenerator.cpp
        ManufacturerTestAcceptor.cpp
        ../irpmodel/Manufacturer.cpp
        ../irp-iso/irpdomain/IrpEvent.cpp
)

target_include_directories(manufacturer_test PRIVATE
        "../irp-iso/irpdomain"
        "../irpmodel"
        "${DEVS_SF_DIR}/include"
)

target_compile_definitions(manufacturer_test PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../data")

target_link_libraries(manufacturer_test PUBLIC IrpModel nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(manufacturer_test PRIVATE /W4)
else()
    target_compile_options(manufacturer_test PRIVATE -Wall -Wextra -pedantic)
endif()

# ----------------------------
# Vehicle Test
# ----------------------------
add_executable(vehicle_test
        vehicle_test.cpp
        VehicleTestGenerator.cpp
        VehicleTestAcceptor.cpp
        ../irpmodel/Vehicle.cpp
        ../irp-iso/irpdomain/IrpEvent.cpp
)

target_include_directories(vehicle_test PRIVATE
        "../irp-iso/irpdomain"
        "../irpmodel"
        "${DEVS_SF_DIR}/include"
)

target_compile_definitions(vehicle_test PRIVATE TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../../data")

target_link_libraries(vehicle_test PUBLIC IrpModel nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(vehicle_test PRIVATE /W4)
else()
    target_compile_options(vehicle_test PRIVATE -Wall -Wextra -pedantic)
endif()
