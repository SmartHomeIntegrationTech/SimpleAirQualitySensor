/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include <Arduino.h>
#include <Preferences.h>
#include <SHIBME680.h>
#include <SHIHardware.h>
#include <SHIMQTT.h>
#include <SHIMulticastHandler.h>
#include <SHIOpenhabRestCommunicator.h>

SHI::BME680Config bmeConfig;
std::shared_ptr<SHI::BME680> bme680 = std::make_shared<SHI::BME680>(bmeConfig);
SHI::OpenhabRestCommunicatorConfig ohConfig;
std::shared_ptr<SHI::OpenhabRestCommunicator> comms =
    std::make_shared<SHI::OpenhabRestCommunicator>(ohConfig);
SHI::MQTTConfig mqttConfig;
std::shared_ptr<SHI::MQTT> mqtt = std::make_shared<SHI::MQTT>(mqttConfig);
SHI::MulticastHandlerConfig multicastConfig;
std::shared_ptr<SHI::MulticastHandler> multicastComms =
    std::make_shared<SHI::MulticastHandler>(multicastConfig);

void setup() {
  static String tempName = "";
  SHI::hw->addCommunicator(comms);
  SHI::hw->addCommunicator(mqtt);
  SHI::hw->addCommunicator(multicastComms);
  SHI::hw->addSensor(bme680);
  tempName = "AirQualityMobile" + WiFi.macAddress();
  tempName.replace(".", "_");
  SHI::hw->setup(tempName.c_str());
}

// Function that is looped forever
void loop(void) {
  uint32_t loopStart = millis();
  SHI::hw->loop();
  int diff = millis() - loopStart;
  if (diff < 1000) {
    delay(1000 - diff);
  }
}
