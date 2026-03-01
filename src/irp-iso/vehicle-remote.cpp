#define NOMINMAX
#include "VehicleStreamingSimulator.h"
#include "devs/msg/DevsMessageFactory.h"
#include "devs/devskafka/KafkaCoordinator.h"
#include "devs/devskafka/KafkaSimulatorRouter.h"
#include "IrpData.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "data"
#endif

int main(int argc, char** argv) {
    const std::string simulationId = "BusyMartSimulation";
    const Topic receiverTopic = "irp-system";
    const Topic producerTopic = "irp-system";
    const std::string brokers = "localhost:29092";

    std::cout << "There are " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    const Properties props({
                           {"bootstrap.servers", {brokers}},
                           {"security.protocol", {"PLAINTEXT"}}
        });

    initializeDefaultPortValueFactories();
    PortValueMessageRegistry::getInstance().registerCustomCreator(
        "iso.example.irpsystem.irpdomain.ImmutableDelivery",
        [](nlohmann::json jv) {
            return new PortValueMessage<IrpEvent *, std::string>(
                jv.at("portName").get<std::string>(),
                "iso.example.irpsystem.irpdomain.ImmutableDelivery",
                IrpEvent::fromJson(jv.at("value"))
            );
        });
    PortValueMessageRegistry::getInstance().registerCustomCreator(
        "iso.example.irpsystem.irpdomain.ImmutableVehicleCost",
        [](nlohmann::json jv) {
            return new PortValueMessage<IrpEvent *, std::string>(
                jv.at("portName").get<std::string>(),
                "iso.example.irpsystem.irpdomain.ImmutableVehicleCost",
                IrpEvent::fromJson(jv.at("value"))
            );
        });
    PortValueMessageRegistry::getInstance().registerCustomCreator(
        "iso.example.irpsystem.irpdomain.ImmutableDeliveryRoute",
        [](nlohmann::json jv) {
            return new PortValueMessage<IrpEvent *, std::string>(
                jv.at("portName").get<std::string>(),
                "iso.example.irpsystem.irpdomain.ImmutableDeliveryRoute",
                IrpEvent::fromJson(jv.at("value"))
            );
        });

    DevsMessageFactory *devsMessageFactory = new DevsMessageFactory();

    KafkaCoordinator *kafkaCoordinator = new KafkaCoordinator(props, producerTopic);

    // 0. Load IrpData
    std::string dataPath = std::string(TEST_DATA_DIR) + "/S_abs1n5_2_L3.json";
    const IrpData irp_data = IrpData::fromJsonFile(dataPath);
    std::cout << "Loaded IrpData: numNodes=" << irp_data.numNodes
              << ", numTimePeriods=" << irp_data.numTimePeriods << "\n";

    // 1. Setup Vehicle properties from IrpData
    const Coordinate base(irp_data.manufacturer.x, irp_data.manufacturer.y);
    const int vehicleId = 1;
    const double capacity = irp_data.vehicleCapacity;
    const double startingDailyKm = 0.0;
    const double costPerKm = irp_data.vehicleCostPerKm;
    const double speedKmHr = irp_data.vehicleSpeedKmHr;
    DeliveryRoute emptyRoute(1, {});

    Vehicle *vehicle = new Vehicle(
        vehicleId,
        capacity,
        startingDailyKm,
        emptyRoute,
        base,
        costPerKm,
        speedKmHr
    );

    auto* vehicleStreamingSimulator =
        new VehicleStreamingSimulator(kafkaCoordinator, devs_message::LONG_SIM_TIME,
            "vehicle1", vehicle);

    auto* kafkaSimulatorRouter = new KafkaSimulatorRouter(props, receiverTopic, devsMessageFactory);
    kafkaSimulatorRouter->registerSimulator("vehicle1", vehicleStreamingSimulator);

    int runMinutes = 5;
    for (int i = 0; i < 60 * runMinutes; i ++) {
        auto start = std::chrono::system_clock::now().time_since_epoch();
        auto millisStart = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();
        bool polledForOneSecond = false;
        while (!polledForOneSecond) {
            kafkaSimulatorRouter->poll();
            auto check = std::chrono::system_clock::now().time_since_epoch();
            auto millisCheck = std::chrono::duration_cast<std::chrono::milliseconds>(check).count();
            if (millisCheck - millisStart >= 1000) {
                polledForOneSecond = true;
            }
        }
        std::cout << "slept for " << i << " seconds" << std::endl;
    }
    kafkaSimulatorRouter->setRunning(false);
}
