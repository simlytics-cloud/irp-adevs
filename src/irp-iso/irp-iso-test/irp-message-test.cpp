
#include "devs/msg/DevsMessage.h"
#include "devs/msg/PortValueMessage.h"
#include "devs/msg/DevsMessageFactory.h"
#include "../irpdomain/IrpEvent.h"
#include "adevs.h"
#include "../irpdomain/Delivery.h"
#include "../irpdomain/InventoryCost.h"
#include "../irpdomain/VehicleCost.h"


using namespace adevs;
using namespace devs_message;


int main(int argc, char** argv) {

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
	PortValueMessageRegistry::getInstance().registerCustomCreator(
		"iso.example.irpsystem.irpdomain.ImmutableVehicleCost",
		[](nlohmann::json jv) {
			return new PortValueMessage<IrpEvent *, std::string>(
				jv.at("portName").get<std::string>(),
				"iso.example.irpsystem.irpdomain.ImmutableVehicleCost",
				IrpEvent::fromJson(jv.at("value"))
			);
		});


	DevsMessageFactory* devsMessageFactory = new DevsMessageFactory();


    // From Java
    const char* executeTransitionJava = R"JSON(
{
	"messageType": "ExecuteTransition",
	"eventTime": 645,
	"payload": {
		"@class": "devs.iso.ExecuteTransitionPayload",
		"inputs": [
			{
				"value": {
					"@class": "iso.example.irpsystem.irpdomain.ImmutableDelivery",
					"retailerId": 2,
					"retailerLocation": {
						"@class": "iso.example.irpsystem.irpdomain.ImmutableCoordinate",
						"x": 267.0,
						"y": 87.0
					},
					"productAmount": 35.0
				},
				"portName": "receiveDelivery"
			}
		]
	},
	"simulationId": "InventoryRoutingApp",
	"messageId": "0dd3a4cd-27ea-4d60-9c67-74c1cad3ead1",
	"senderId": "inventoryRouting",
	"receiverId": "retailer2"
}
)JSON";
    json jv = json::parse(executeTransitionJava);

    DevsMessage* devsMessage = devsMessageFactory->fromJsonValue(jv);
    ExecuteTransition* executeTransition;
    if (devsMessage->getDevsMessageType() == DevsMessage::EXECUTE_TRANSITION) {
        executeTransition = dynamic_cast<ExecuteTransition*>(devsMessage);
    } else {
        throw std::invalid_argument("Execute Transition message was not ExecuteTransition");
    }

	std::vector<PortValueMessageBase*> inputs = executeTransition->getInputs();

	if (inputs.empty() || inputs[0] == nullptr) {
		throw std::invalid_argument("ExecuteTransition inputs were empty (expected a Delivery on an input port)");
	}

	auto* input0 = dynamic_cast<PortValueMessage<IrpEvent*, std::string>*>(inputs[0]);
	if (input0 == nullptr) {
		throw std::invalid_argument("First input was not a PortValueMessage<IrpEvent*, std::string>");
	}

	IrpEvent* delivery = input0->getValue();
	auto* deliveryObj = dynamic_cast<Delivery*>(delivery);
	if (deliveryObj == nullptr) {
		throw std::invalid_argument("Delivery message was not Delivery");
	}
	if (deliveryObj->retailerId != 2) {
		throw std::invalid_argument("Delivery message had wrong retailerId");
	}
	if (deliveryObj->productAmount != 35.0) {
		throw std::invalid_argument("Delivery message had wrong productAmount");
	}
	if (deliveryObj->retailerLocation.x != 267.0 || deliveryObj->retailerLocation.y != 87.0) {
		throw std::invalid_argument("Delivery message had wrong retailerLocation");
	}

	// Now test an output message

	jv = json::parse(R"JSON(
{
	"messageType": "OutputReport",
	"eventTime": 960,
	"payload": {
		"@class": "devs.iso.OutputReportPayload",
		"outputs": [
			{
				"value": {
					"@class": "iso.example.irpsystem.irpdomain.ImmutableInventoryCost",
					"day": 1,
					"retailerId": 2,
					"cost": 2.1
				},
				"portName": "dailyInventoryCost"
			}
		]
	},
	"simulationId": "InventoryRoutingApp",
	"messageId": "488317be-cb6f-4cbb-abe3-72f731f96664",
	"senderId": "retailer2",
	"receiverId": "inventoryRouting",
	"nextInternalTime": 960
})JSON");

	devsMessage = devsMessageFactory->fromJsonValue(jv);
	OutputReport* outputReport;
	if (devsMessage->getDevsMessageType() == DevsMessage::OUTPUT_REPORT) {
		outputReport = dynamic_cast<OutputReport*>(devsMessage);
	} else {
		throw std::invalid_argument("Output report message was not OutputReport");
	}

	std::vector<PortValueMessageBase*> outputs = outputReport->getOutputs();

	if (outputs.empty() || outputs[0] == nullptr) {
		throw std::invalid_argument("OutputReport outputs were empty (expected a InventoryCost on an output port)");
	}

	auto* output0 = dynamic_cast<PortValueMessage<IrpEvent*, std::string>*>(outputs[0]);
	if (output0 == nullptr) {
		throw std::invalid_argument("First output was not a PortValueMessage<IrpEvent*, std::string>");
	}

	IrpEvent* irpEvent = output0->getValue();
	auto* inventoryCost = dynamic_cast<InventoryCost*>(irpEvent);

	if (inventoryCost == nullptr) {
		throw std::invalid_argument("InventoryCost message was not InventoryCost");
	}
	if (inventoryCost->day != 1) {
		throw std::invalid_argument("InventoryCost message had wrong day");
	}
	if (inventoryCost->retailerId != 2) {
		throw std::invalid_argument("InventoryCost message had wrong retailerId");
	}
	if (inventoryCost->cost != 2.1) {
		throw std::invalid_argument("InventoryCost message had wrong cost");
	}

	// Test VehicleCost
	jv = json::parse(R"JSON(
{
	"messageType": "OutputReport",
	"eventTime": 960,
	"payload": {
		"@class": "devs.iso.OutputReportPayload",
		"outputs": [
			{
				"value": {
					"@class": "iso.example.irpsystem.irpdomain.ImmutableVehicleCost",
					"day": 1,
					"vehicleId": 3,
					"cost": 45.5
				},
				"portName": "dailyVehicleCost"
			}
		]
	},
	"simulationId": "InventoryRoutingApp",
	"messageId": "588317be-cb6f-4cbb-abe3-72f731f96665",
	"senderId": "manufacturer",
	"receiverId": "inventoryRouting",
	"nextInternalTime": 960
})JSON");

	devsMessage = devsMessageFactory->fromJsonValue(jv);
	if (devsMessage->getDevsMessageType() == DevsMessage::OUTPUT_REPORT) {
		outputReport = dynamic_cast<OutputReport*>(devsMessage);
	} else {
		throw std::invalid_argument("Output report message was not OutputReport (VehicleCost)");
	}

	outputs = outputReport->getOutputs();

	if (outputs.empty() || outputs[0] == nullptr) {
		throw std::invalid_argument("OutputReport outputs were empty (expected a VehicleCost)");
	}

	output0 = dynamic_cast<PortValueMessage<IrpEvent*, std::string>*>(outputs[0]);
	irpEvent = output0->getValue();
	auto* vehicleCost = dynamic_cast<VehicleCost*>(irpEvent);

	if (vehicleCost == nullptr) {
		throw std::invalid_argument("VehicleCost message was not VehicleCost");
	}
	if (vehicleCost->day != 1) {
		throw std::invalid_argument("VehicleCost message had wrong day");
	}
	if (vehicleCost->vehicleId != 3) {
		throw std::invalid_argument("VehicleCost message had wrong vehicleId");
	}
	if (vehicleCost->cost != 45.5) {
		throw std::invalid_argument("VehicleCost message had wrong cost");
	}

    cout << "IRP message test succeeded!!" << endl;

    return 0;
}

