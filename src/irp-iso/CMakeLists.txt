
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

