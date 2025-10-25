#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace sensor_bmi270 {

class BMI270Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  // constructeur, intervalle
  explicit BMI270Sensor(uint32_t update_interval = 60000);

  void setup() override;
  void dump_config() override;
  void update() override;

  // capteurs à exposer
  sensor::Sensor *accel_x_{nullptr};
  sensor::Sensor *accel_y_{nullptr};
  sensor::Sensor *accel_z_{nullptr};
  sensor::Sensor *gyro_x_{nullptr};
  sensor::Sensor *gyro_y_{nullptr};
  sensor::Sensor *gyro_z_{nullptr};

  // configuration
  void set_address(uint8_t addr) { this->addr_ = addr; }

 protected:
  uint8_t addr_{0x68};
};

}  // namespace sensor_bmi270
}  // namespace esphome
