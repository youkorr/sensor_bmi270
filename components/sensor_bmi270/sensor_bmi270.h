#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace sensor_bmi270 {

class BMI270Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
public:
  BMI270Sensor() = default;

  void setup() override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Setters pour sous-capteurs
  void set_accel_x_sensor(sensor::Sensor *s) { accel_x_ = s; }
  void set_accel_y_sensor(sensor::Sensor *s) { accel_y_ = s; }
  void set_accel_z_sensor(sensor::Sensor *s) { accel_z_ = s; }
  void set_gyro_x_sensor(sensor::Sensor *s) { gyro_x_ = s; }
  void set_gyro_y_sensor(sensor::Sensor *s) { gyro_y_ = s; }
  void set_gyro_z_sensor(sensor::Sensor *s) { gyro_z_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_ = s; }

 protected:
protected:
  bool read_raw_data();
  bool initialise_device_();
  bool upload_config_file_();
  bool soft_reset_();
  bool set_pmu_state_();

  sensor::Sensor *accel_x_{nullptr};
  sensor::Sensor *accel_y_{nullptr};
  sensor::Sensor *accel_z_{nullptr};
  sensor::Sensor *gyro_x_{nullptr};
  sensor::Sensor *gyro_y_{nullptr};
  sensor::Sensor *gyro_z_{nullptr};
  sensor::Sensor *temperature_{nullptr};

  int16_t raw_ax_{0}, raw_ay_{0}, raw_az_{0};
  int16_t raw_gx_{0}, raw_gy_{0}, raw_gz_{0};
  int16_t raw_temperature_{0};
  bool temperature_valid_{false};
};

}  // namespace sensor_bmi270
}  // namespace esphome


