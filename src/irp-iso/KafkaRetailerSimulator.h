#pragma once

#include <devs/msg/DevsMessageFactory.h>
#include <kafka/KafkaConsumer.h>
#include "devs/devskafka/KafkaReceiverSimulator.h"
#include "RetailerStreamingSimulator.h"
#include "devs/DevsSfCoordinator.h"
#include "devs/msg/DevsMessageFactory.h"

using namespace kafka;
using namespace kafka::clients::consumer;

class KafkaRetailerSimulator: public KafkaReceiverSimulator {

public:
    explicit KafkaRetailerSimulator(const Properties& properties, RetailerStreamingSimulator* simulator, const Topic& topic,
                        std::string receiverId, DevsMessageFactory* devsMessageFactory);

protected:
    void handleMessage(std::string messageFromKafka) override;

private:
    RetailerStreamingSimulator* simulator;
    DevsMessageFactory* devsMessageFactory;
    std::string receiverId;
};


