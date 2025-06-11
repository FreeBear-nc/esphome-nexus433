/*
SPDX-License-Identifier: GPL-3.0-or-later
SPDX-FileCopyrightText: © 2025 FreeBear-nc https://github.com/FreeBear-nc
*/

#include "esphome/core/log.h"
#include "nexus433.h"

#define PAYLOAD_LEN 64
#define TOPIC_LEN 64


namespace esphome {
namespace nexus433 {

using namespace mqtt;

static const char *TAG = "nexus433.component";
NEXUS_DATA nexus_data_prev;

void Nexus433::setup() {
    ESP_LOGI(TAG, "Calling nexus setup\n");
    this->pin_->get_pin();
    ESP_LOGI(TAG, "Using GPIO%i for interrupt", this->pin_->get_pin());
    config_receiver((int)this->pin_->get_pin());
}

char payload[PAYLOAD_LEN];
char topic[TOPIC_LEN];

int generate_payload(void)
{
    NEXUS_DATA nexus_data;

    nexus_data = decode_nexus_data();

    if ((nexus_data_prev.ID == nexus_data.ID) && ((nexus_data.Timestamp - nexus_data_prev.Timestamp) < 800) ) {
        return -1;
    } else {
        const char* const payload_fmt = "{ "
                "\"temperature\": %s, "
                "\"humidity\": %d, "
                "\"battery\": \"%i\", "
                "}";

        const char* const topic_fmt = "nexus433/sensor/%02x%02x/state";
        char str_temp[6];
        dtostrf(nexus_data.Temperature, 3, 1, str_temp);
        snprintf(payload, sizeof(payload),
                payload_fmt, str_temp, nexus_data.Humidity, nexus_data.Battery * 100);

        snprintf(topic, sizeof(topic), topic_fmt, nexus_data.ID, nexus_data.Channel);
        ESP_LOGI(TAG, "publish %s %s", topic, payload);
    }

    nexus_data_prev = nexus_data;
    return 0;
}

void Nexus433::loop() {
    int err;
    if (!int_enable) {
        err = generate_payload();
        if (!err) this->publish(topic, payload);
        int_enable = 1;
    }
}

void Nexus433::dump_config(){
    ESP_LOGCONFIG(TAG, "Nexus433 component");
    LOG_PIN("  Pin: ", this->pin_);
}

}  // namespace nexus433
}  // namespace esphome
