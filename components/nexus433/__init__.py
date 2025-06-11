"""
SPDX-License-Identifier: MIT
SPDX-FileCopyrightText: © 2025 FreeBear-nc https://github.com/FreeBear-nc
"""

import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import mqtt
from esphome.const import CONF_ID, CONF_PIN
from esphome.cpp_helpers import gpio_pin_expression


_LOGGER = logging.getLogger(__name__)
DEPENDENCIES = ["mqtt"]
AUTO_LOAD = ["mqtt"]

nexus433_ns = cg.esphome_ns.namespace("nexus433")
Nexus433 = nexus433_ns.class_("Nexus433", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Nexus433),
        cv.Required(CONF_PIN): pins.internal_gpio_input_pin_schema,

    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    pin = await gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_input_pin(pin))
