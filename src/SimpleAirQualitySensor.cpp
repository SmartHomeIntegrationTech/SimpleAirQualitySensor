/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include <Arduino.h>
#include <Preferences.h>
// #include <SHIBLEBeacon.h>
#include <SHIBME280.h>
#include <SHIBME680.h>
#include <SHIESP32HW.h>
#include <SHIHardware.h>
#include <SHIMQTT.h>
#include <SHIMulticastHandler.h>
#include <SHIOpenhabRestCommunicator.h>
#include <SHIWifiSensor.h>

SHI::ESP32HWConfig espConfig;
std::shared_ptr<SHI::ESP32HW> esphw = std::make_shared<SHI::ESP32HW>(espConfig);
#ifdef DUAL_CHANNEL
SHI::BME280Config indoorConfig;
SHI::BME280Config outdoorConfig;
std::shared_ptr<SHI::BME280> indoor =
    std::make_shared<SHI::BME280>(indoorConfig);
std::shared_ptr<SHI::BME280> outdoor =
    std::make_shared<SHI::BME280>(outdoorConfig);
std::shared_ptr<SHI::SensorGroup> indoorGroup =
    std::make_shared<SHI::SensorGroup>("indoor");
std::shared_ptr<SHI::SensorGroup> outdoorGroup =
    std::make_shared<SHI::SensorGroup>("outdoor");
#else
SHI::BME680Config bmeConfig;
std::shared_ptr<SHI::BME680> bme680 = std::make_shared<SHI::BME680>(bmeConfig);
#endif
std::shared_ptr<SHI::WifiSensor> wifisensor =
    std::make_shared<SHI::WifiSensor>();
SHI::OpenhabRestCommunicatorConfig ohConfig;
std::shared_ptr<SHI::OpenhabRestCommunicator> comms =
    std::make_shared<SHI::OpenhabRestCommunicator>(ohConfig);
SHI::MQTTConfig mqttConfig;
std::shared_ptr<SHI::MQTT> mqtt = std::make_shared<SHI::MQTT>(mqttConfig);
SHI::MulticastHandlerConfig multicastConfig;
std::shared_ptr<SHI::MulticastHandler> multicastComms =
    std::make_shared<SHI::MulticastHandler>(multicastConfig);

void setup() {
  ets_printf("Entering setup\n");
  SHI::hw = esphw.get();
  static String tempName = "";
  SHI::hw->addCommunicator(comms);
  SHI::hw->addCommunicator(mqtt);
  SHI::hw->addCommunicator(multicastComms);
#ifdef DUAL_CHANNEL
  indoorGroup->addSensor(indoor);
  outdoorGroup->addSensor(outdoor);
  SHI::hw->addSensorGroup(indoorGroup);
  SHI::hw->addSensorGroup(outdoorGroup);
  Wire1.begin(18, 19);
  indoorConfig.useBus = 1;
  indoor->reconfigure(&indoorConfig);
  indoorGroup->addSensor(wifisensor);
#else
  SHI::hw->addSensor(bme680);
  SHI::hw->addSensor(wifisensor);
#endif
  tempName = "AirQualityMobile" + WiFi.macAddress();
  tempName.replace(".", "_");
  SHI::hw->setup(tempName.c_str());
  // SHI::Beacon::setupBeacon();
}

// Function that is looped forever
void loop(void) {
  uint32_t loopStart = millis();
  SHI::hw->loop();
  int diff = millis() - loopStart;
  //  if (diff > 100) {
  // SHI::Beacon::beaconLoop(100);
  //    diff = millis() - loopStart;
  //  }
  if (diff < 1000) {
    delay(1000 - diff);
  }
}
