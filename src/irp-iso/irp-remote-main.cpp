
#define NOMINMAX
#include "RetailerStreamingSimulator.h"
#include "devs/msg/DevsMessageFactory.h"
#include "devs/devskafka/KafkaCoordinator.h"
#include "KafkaRetailerSimulator.h"

int main(int argc, char** argv) {
const std::string simulationId = "BusyMartSimulation";
    const Topic receiverTopic = "irp-system";
    const Topic producerTopic = "irp-system";
    const std::string brokers = "localhost:29092";
    //const std::string brokers = "pkc-2396y.us-east-1.aws.confluent.cloud:9092";

    std::cout << "There are " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "Agrument " << i << ": " << argv[i] << std::endl;
    }

    // if (argc != 3) {
    //     std::cout << "Needs two inputs to example consumer, the username and password for Kafka consumer." << std::endl;
    //     //throw std::invalid_argument("Needs two inputs to example consumer, the username and password for Kafka consumer.");
    // }

    // Prepare the configuration
    //const Properties props({ {"bootstrap.servers", {brokers}},
    //                        {"security.protocol", {"SASL_SSL"}},
    //                        {"sasl.username", {argv[1]}},
    //                        {"sasl.password", {argv[2]}},
    //                        {"sasl.mechanisms", {"PLAIN"}}
    //    });

    const Properties props({
                           {"bootstrap.servers", {brokers}},  // Local Kafka broker address
                           {"security.protocol", {"PLAINTEXT"}}       // No SSL or SASL, use plaintext communication
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
        "iso.example.irpsystem.irpdomain.ImmutableInventoryCost",
        [](nlohmann::json jv) {
            return new PortValueMessage<IrpEvent *, std::string>(
                jv.at("portName").get<std::string>(),
                "iso.example.irpsystem.irpdomain.ImmutableInventoryCost",
                IrpEvent::fromJson(jv.at("value"))
            );
        });

    DevsMessageFactory *devsMessageFactory = new DevsMessageFactory();

    KafkaCoordinator *kafkaCoordinator = new KafkaCoordinator(props, producerTopic);
    const Coordinate retailerCoord(172.0, 334.0);
    const FacilityProps facility_props(retailerCoord, /*startingInventory*/ 70.0, /*inventoryCost*/ 0.03);

    const int retailerId = 2;

    // Retailer
    Retailer *retailer = new Retailer(
        facility_props,
        /*currentInventory*/
        retailerId,
        /*minInventory*/
        0.0,
        /*maxInventory*/
        105.0,
        /*dailyConsumption*/
        35.0
    );

    RetailerStreamingSimulator* retailerStreamingSimulator =
        new RetailerStreamingSimulator(kafkaCoordinator, devs_message::LONG_SIM_TIME,
            "retailer2", retailer);
    KafkaRetailerSimulator* kafkaRetailerSimulator = new KafkaRetailerSimulator(props,
        retailerStreamingSimulator, receiverTopic, "retailer2",devsMessageFactory);
    int runMinutes = 5;
    for (int i = 0; i < 60 * runMinutes; i ++) {
        auto start = std::chrono::system_clock::now().time_since_epoch();
        auto millisStart = std::chrono::duration_cast<std::chrono::milliseconds>(start).count();
        bool polledForOneSecond = false;
        while (!polledForOneSecond) {
            kafkaRetailerSimulator->poll();
            auto check = std::chrono::system_clock::now().time_since_epoch();
            auto millisCheck = std::chrono::duration_cast<std::chrono::milliseconds>(check).count();
            if (millisCheck - millisStart >= 1000) {
                polledForOneSecond = true;
            }
        }
        cout << "slept for " << i << " seconds" << endl;
    }
    kafkaRetailerSimulator->setRunning(false);
}
