#include "sensor_bmi270.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sensor_bmi270 {

static const char *const TAG = "sensor_bmi270";

// Registres de base
static const uint8_t BMI270_REG_CHIP_ID = 0x00;
static const uint8_t BMI270_REG_ACCEL_X_LSB = 0x0C;
static const uint8_t BMI270_CHIP_ID = 0x24;

void BMI270Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Initialisation du BMI270...");

  uint8_t id = 0;
  if (!this->read_register(BMI270_REG_CHIP_ID, &id, 1)) {
    ESP_LOGE(TAG, "Échec lecture ID du capteur !");
    this->mark_failed();
    return;
  }

  if (id != BMI270_CHIP_ID) {
    ESP_LOGE(TAG, "ID inattendu 0x%02X (attendu 0x%02X)", id, BMI270_CHIP_ID);
    this->mark_failed();
    return;
  }

  ESP_LOGI(TAG, "BMI270 détecté à 0x%02X", this->address_);
}

void BMI270Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "BMI270 Sensor:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Le capteur est en erreur !");
  }
}

void BMI270Sensor::update() {
  if (!this->read_raw_data()) {
    ESP_LOGW(TAG, "Échec de lecture BMI270");
    return;
  }

  constexpr float accel_scale = 9.80665f / 16384.0f;  // ±2g
  constexpr float gyro_scale = 1.0f / 131.0f;         // ±250°/s

  if (accel_x_ != nullptr) accel_x_->publish_state(raw_ax_ * accel_scale);
  if (accel_y_ != nullptr) accel_y_->publish_state(raw_ay_ * accel_scale);
  if (accel_z_ != nullptr) accel_z_->publish_state(raw_az_ * accel_scale);

  if (gyro_x_ != nullptr) gyro_x_->publish_state(raw_gx_ * gyro_scale);
  if (gyro_y_ != nullptr) gyro_y_->publish_state(raw_gy_ * gyro_scale);
  if (gyro_z_ != nullptr) gyro_z_->publish_state(raw_gz_ * gyro_scale);
}

bool BMI270Sensor::read_raw_data() {
  uint8_t buffer[12];
  if (!this->read_register(BMI270_REG_ACCEL_X_LSB, buffer, sizeof(buffer))) {
    ESP_LOGE(TAG, "Erreur I2C lecture données BMI270");
    return false;
  }

  raw_ax_ = (int16_t)((buffer[1] << 8) | buffer[0]);
  raw_ay_ = (int16_t)((buffer[3] << 8) | buffer[2]);
  raw_az_ = (int16_t)((buffer[5] << 8) | buffer[4]);
  raw_gx_ = (int16_t)((buffer[7] << 8) | buffer[6]);
  raw_gy_ = (int16_t)((buffer[9] << 8) | buffer[8]);
  raw_gz_ = (int16_t)((buffer[11] << 8) | buffer[10]);

  ESP_LOGV(TAG, "RAW AX:%d AY:%d AZ:%d GX:%d GY:%d GZ:%d",
           raw_ax_, raw_ay_, raw_az_, raw_gx_, raw_gy_, raw_gz_);
  return true;
}

}  // namespace sensor_bmi270
}  // namespace esphome



