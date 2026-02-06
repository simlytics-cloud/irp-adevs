//
// Created by Robert Kewley on 8/14/23.
//

#include <thread>
#include <utility>
#include "KafkaRetailerSimulator.h"



KafkaRetailerSimulator::KafkaRetailerSimulator(const Properties &properties, RetailerStreamingSimulator* simulator,
                                         const Topic &topic, std::string receiverId, DevsMessageFactory* devsMessageFactory):
        KafkaReceiverSimulator(properties, topic), simulator(simulator), 
        devsMessageFactory(devsMessageFactory), receiverId(std::move(receiverId)){

}

void KafkaRetailerSimulator::handleMessage(std::string messageFromKafka) {
    json jv = json::parse(messageFromKafka);
    DevsMessage* devsMessage = devsMessageFactory->fromJsonValue(jv);

    auto *devsSimMessage = dynamic_cast<DevsSimMessage *>(devsMessage);
    if (devsSimMessage != nullptr) {
        if (devsSimMessage->getReceiverId() == receiverId) {
            simulator->acceptMessage(*devsMessage);
        }
    } else {
        std::cerr << "ERROR: Received message is not a DevsSimMessage. Message content: "
                << messageFromKafka << std::endl;
    }
}


