/*
SPDX-License-Identifier: GPL-3.0-or-later
SPDX-FileCopyrightText: © 2025 FreeBear-nc https://github.com/FreeBear-nc
*/

#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/mqtt/custom_mqtt_device.h"
#include "Nexus_Decoder.h"

namespace esphome {
namespace nexus433 {

class Nexus433 : public Component, public mqtt::CustomMQTTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_input_pin(InternalGPIOPin *pin) {this->pin_ = pin; }


 protected:
  InternalGPIOPin *pin_;

};

}  // namespace NEXUS433
}  // namespace esphome