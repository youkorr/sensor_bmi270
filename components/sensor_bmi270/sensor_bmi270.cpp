#include "sensor_bmi270.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sensor_bmi270 {

static const char *TAG = "sensor.bmi270";

BMI270Sensor::BMI270Sensor(uint32_t update_interval)
    : PollingComponent(update_interval), I2CDevice(0x68) {
}

void BMI270Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up BMI270 sensor...");
  // initialisation i2c, vérifier communication
  if (!this->check_address(addr_)) {
    ESP_LOGE(TAG, "BMI270 not found at address 0x%02X", this->addr_);
    this->mark_failed();
    return;
  }
  // … code d'initialisation (reset, config, etc) …
}

void BMI270Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "BMI270:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->addr_);
}

void BMI270Sensor::update() {
  // lire les valeurs accel + gyro
  int16_t ax, ay, az, gx, gy, gz;
  // par ex fonctions de lecture via i2c
  // this->read_register(..., &ax, ...);

  // publier
  this->accel_x_->publish_state(ax * 0.001f);  // conversion selon unité
  this->accel_y_->publish_state(ay * 0.001f);
  this->accel_z_->publish_state(az * 0.001f);
  this->gyro_x_->publish_state(gx * 0.01f);
  this->gyro_y_->publish_state(gy * 0.01f);
  this->gyro_z_->publish_state(gz * 0.01f);
}

}  // namespace sensor_bmi270
}  // namespace esphome
