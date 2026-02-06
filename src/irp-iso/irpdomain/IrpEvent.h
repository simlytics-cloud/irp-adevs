#pragma once

#include "devs/JsonSerializable.h"
#include <nlohmann/json.hpp>

class IrpEvent : public JsonSerializable {
  public:
    ~IrpEvent() override = default; // enables safe polymorphic deletion

    json toJson() { return toJsonValue(); }
    static IrpEvent* fromJson(const json& j);
};
