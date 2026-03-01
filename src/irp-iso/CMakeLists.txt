
#In order to use ADEVS with ISO collaborative modeling standard, we need to publish
#  and subscribe to DEVS models via Kafka

# Requires rdkafka
find_package(PkgConfig REQUIRED)
pkg_check_modules(RdKafka REQUIRED IMPORTED_TARGET rdkafka++ rdkafka)
message(STATUS "librdkafka include directory: ${RdKafka_INCLUDE_DIRS}")
message(STATUS "librdkafka libraries: ${RdKafka_LIBRARIES}")
include_directories(${RdKafka_INCLUDE_DIRS})
link_libraries(PkgConfig::RdKafka)

# Requires modern kafka cpp
set(modern-cpp-kafka_INCLUDE_DIRS "/Users/robertkewley/Library/CloudStorage/OneDrive-simlytics.cloud/Simlytics/dev/simlytics/devs/modern-cpp-kafka/include")
message(STATUS "modern-cpp-kafka include directory: ${modern-cpp-kafka_INCLUDE_DIRS}")
include_directories(${modern-cpp-kafka_INCLUDE_DIRS})

# Requires DEVS Streaming Framework CPP
set(DEVS_SF_DIR "/Users/robertkewley/Library/CloudStorage/OneDrive-simlytics.cloud/Simlytics/dev/simlytics/devs/cpp/devs-sf-cpp/_install")


add_executable(retailer_remote
        retailer-remote.cpp
        RetailerStreamingSimulator.cpp
)

target_include_directories(retailer_remote PRIVATE
        "${DEVS_SF_DIR}/include"
        "${DEVS_SF_DIR}/include/devs"
        "irpdomain"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)

target_compile_definitions(retailer_remote PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

target_link_libraries(retailer_remote PUBLIC
        "${DEVS_SF_DIR}/lib/libdevs_sf.a"
        nlohmann_json::nlohmann_json
        IrpModel
)

add_executable(manufacturer-remote
        manufacturer-remote.cpp
        ManufacturerStreamingSimulator.cpp
)

target_include_directories(manufacturer-remote PRIVATE
        "${DEVS_SF_DIR}/include"
        "${DEVS_SF_DIR}/include/devs"
        "irpdomain"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)

target_compile_definitions(manufacturer-remote PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

target_link_libraries(manufacturer-remote PUBLIC
        "${DEVS_SF_DIR}/lib/libdevs_sf.a"
        nlohmann_json::nlohmann_json
        IrpModel
)

add_executable(vehicle-remote
        vehicle-remote.cpp
        VehicleStreamingSimulator.cpp
)

target_include_directories(vehicle-remote PRIVATE
        "${DEVS_SF_DIR}/include"
        "${DEVS_SF_DIR}/include/devs"
        "irpdomain"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)

target_compile_definitions(vehicle-remote PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

target_link_libraries(vehicle-remote PUBLIC
        "${DEVS_SF_DIR}/lib/libdevs_sf.a"
        nlohmann_json::nlohmann_json
        IrpModel
)

add_executable(irp-message-test irp-iso-test/irp-message-test.cpp
      irpdomain/Coordinate.h
        irpdomain/Delivery.h
        irpdomain/FacilityProps.h
        irpdomain/InventoryCost.h
        irpdomain/IrpEvent.h
        irpdomain/IrpEvent.cpp
)

target_include_directories(irp-message-test PRIVATE
        "${DEVS_SF_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)

target_link_libraries(irp-message-test PUBLIC
        "${DEVS_SF_DIR}/lib/libdevs_sf.a"
        nlohmann_json::nlohmann_json
)

add_executable(irp-delivery-route-test irpdomaintest/test_delivery_route.cpp)
target_include_directories(irp-delivery-route-test PRIVATE
        "${DEVS_SF_DIR}/include"
        "irpdomain"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)
target_link_libraries(irp-delivery-route-test PUBLIC
        IrpModel
        nlohmann_json::nlohmann_json
)

add_executable(irp-delivery-schedule-test irpdomaintest/test_delivery_schedule.cpp)
target_include_directories(irp-delivery-schedule-test PRIVATE
        "${DEVS_SF_DIR}/include"
        "irpdomain"
        "${PROJECT_SOURCE_DIR}/src/irpmodel"
)
target_link_libraries(irp-delivery-schedule-test PUBLIC
        IrpModel
        nlohmann_json::nlohmann_json
)
