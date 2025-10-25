#include "sensor_bmi270.h"
#include "bmi270_config_file.h"

#include "esphome/core/log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>

namespace esphome {
namespace sensor_bmi270 {

static const char *const TAG = "sensor_bmi270";

// Registres BMI270
static const uint8_t BMI270_REG_CHIP_ID   = 0x00;
static const uint8_t BMI270_REG_CMD       = 0x7E;   // Command register
static const uint8_t BMI270_CMD_SOFTRESET = 0xB6;   // Soft reset
static const uint8_t BMI270_CHIP_ID       = 0x24;
namespace {

// Bloc de données : accel x/y/z (6 octets) + gyro x/y/z (6 octets)
static const uint8_t BMI270_REG_DATA      = 0x0C;
static const uint8_t BMI270_DATA_LEN      = 12;
constexpr uint8_t BMI270_REG_CHIP_ID = 0x00;
constexpr uint8_t BMI270_REG_ERR_REG = 0x02;
constexpr uint8_t BMI270_REG_STATUS = 0x21;
constexpr uint8_t BMI270_REG_PMU_STATUS = 0x03;
constexpr uint8_t BMI270_REG_ACC_X_LSB = 0x0C;
constexpr uint8_t BMI270_REG_TEMP_LSB = 0x22;
constexpr uint8_t BMI270_REG_ACC_CONF = 0x40;
constexpr uint8_t BMI270_REG_ACC_RANGE = 0x41;
constexpr uint8_t BMI270_REG_GYR_CONF = 0x42;
constexpr uint8_t BMI270_REG_GYR_RANGE = 0x43;
constexpr uint8_t BMI270_REG_INIT_CTRL = 0x59;
constexpr uint8_t BMI270_REG_INIT_ADDR_LSB = 0x5B;
constexpr uint8_t BMI270_REG_INIT_ADDR_MSB = 0x5C;
constexpr uint8_t BMI270_REG_INIT_DATA = 0x5E;
constexpr uint8_t BMI270_REG_PWR_CONF = 0x7C;
constexpr uint8_t BMI270_REG_PWR_CTRL = 0x7D;
constexpr uint8_t BMI270_REG_CMD = 0x7E;

void BMI270Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Initialisation du BMI270...");
constexpr uint8_t BMI270_CMD_SOFTRESET = 0xB6;
constexpr uint8_t BMI270_CHIP_ID = 0x24;

  // Attente d’alimentation stable
  vTaskDelay(pdMS_TO_TICKS(5));
constexpr TickType_t BMI270_RESET_DELAY = pdMS_TO_TICKS(50);
constexpr TickType_t BMI270_POWER_ON_DELAY = pdMS_TO_TICKS(10);
constexpr TickType_t BMI270_SENSOR_STARTUP = pdMS_TO_TICKS(30);

  // ✅ Soft reset recommandé par Bosch
  uint8_t cmd = BMI270_CMD_SOFTRESET;
  if (!this->write_register(BMI270_REG_CMD, &cmd, 1)) {
    ESP_LOGW(TAG, "Impossible d'envoyer le soft-reset (CMD 0x%02X)", BMI270_REG_CMD);
  }
constexpr float ACC_LSB_TO_MPS2 = 9.80665f / 16384.0f;  // ±2 g
constexpr float GYR_LSB_TO_DPS = 2000.0f / 32768.0f;     // ±2000 °/s
constexpr float TEMP_LSB_TO_C = 1.0f / 512.0f;           // datasheet
constexpr float TEMP_OFFSET_C = 23.0f;

  // Attendre que le reset soit appliqué
  vTaskDelay(pdMS_TO_TICKS(50));
bool write_u8(i2c::I2CDevice *dev, uint8_t reg, uint8_t value) {
  return dev->write_register(reg, &value, 1);
}

  // Lecture Chip-ID
  uint8_t id = 0x00;
  if (!this->read_register(BMI270_REG_CHIP_ID, &id, 1)) {
    ESP_LOGE(TAG, "Échec lecture ID du capteur (addr I2C 0x%02X)", this->address_);
    this->mark_failed();
    return;
  }
bool read_u8(i2c::I2CDevice *dev, uint8_t reg, uint8_t &value) {
  return dev->read_register(reg, &value, 1);
}

  ESP_LOGD(TAG, "Chip-ID lu: 0x%02X", id);
}  // namespace

  if (id != BMI270_CHIP_ID) {
    ESP_LOGE(TAG, "ID inattendu 0x%02X (attendu 0x%02X) à l'adresse 0x%02X",
             id, BMI270_CHIP_ID, this->address_);
void BMI270Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Initialisation du BMI270...");

  if (!this->initialise_device_()) {
    ESP_LOGE(TAG, "Impossible d'initialiser le BMI270 à l'adresse 0x%02X", this->address_);
    this->mark_failed();
    return;
  }

  ESP_LOGI(TAG, "✅ BMI270 détecté à l’adresse 0x%02X", this->address_);
  ESP_LOGI(TAG, "✅ BMI270 initialisé à l’adresse 0x%02X", this->address_);
}

void BMI270Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "BMI270 Sensor:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "❌ Le capteur est en erreur !");
  }
}

void BMI270Sensor::update() {
  if (!this->read_raw_data()) {
    ESP_LOGW(TAG, "Échec de lecture des données BMI270");
    return;
  }

  // ⚙️ Échelles approximatives (à ajuster selon ta config range/ODR)
  constexpr float accel_scale = 9.80665f / 16384.0f;  // ±2g
  constexpr float gyro_scale  = 1.0f / 131.0f;        // ±250°/s
  if (accel_x_ != nullptr)
    accel_x_->publish_state(raw_ax_ * ACC_LSB_TO_MPS2);
  if (accel_y_ != nullptr)
    accel_y_->publish_state(raw_ay_ * ACC_LSB_TO_MPS2);
  if (accel_z_ != nullptr)
    accel_z_->publish_state(raw_az_ * ACC_LSB_TO_MPS2);

  if (accel_x_ != nullptr) accel_x_->publish_state(raw_ax_ * accel_scale);
  if (accel_y_ != nullptr) accel_y_->publish_state(raw_ay_ * accel_scale);
  if (accel_z_ != nullptr) accel_z_->publish_state(raw_az_ * accel_scale);
  if (gyro_x_ != nullptr)
    gyro_x_->publish_state(raw_gx_ * GYR_LSB_TO_DPS);
  if (gyro_y_ != nullptr)
    gyro_y_->publish_state(raw_gy_ * GYR_LSB_TO_DPS);
  if (gyro_z_ != nullptr)
    gyro_z_->publish_state(raw_gz_ * GYR_LSB_TO_DPS);

  if (gyro_x_ != nullptr)  gyro_x_->publish_state(raw_gx_ * gyro_scale);
  if (gyro_y_ != nullptr)  gyro_y_->publish_state(raw_gy_ * gyro_scale);
  if (gyro_z_ != nullptr)  gyro_z_->publish_state(raw_gz_ * gyro_scale);
  if (temperature_ != nullptr && temperature_valid_)
    temperature_->publish_state(static_cast<float>(raw_temperature_) * TEMP_LSB_TO_C + TEMP_OFFSET_C);
}

bool BMI270Sensor::read_raw_data() {
  uint8_t buf[BMI270_DATA_LEN] = {0};
  std::array<uint8_t, 12> buffer{};
  raw_temperature_ = 0;
  temperature_valid_ = false;

  if (!this->read_register(BMI270_REG_DATA, buf, BMI270_DATA_LEN)) {
    ESP_LOGE(TAG, "Erreur I2C lors de la lecture des 12 octets de données");
  if (!this->read_register(BMI270_REG_ACC_X_LSB, buffer.data(), buffer.size())) {
    ESP_LOGE(TAG, "Erreur I2C lors de la lecture des données du BMI270");
    return false;
  }

  raw_ax_ = (int16_t)((buf[1]  << 8) | buf[0]);
  raw_ay_ = (int16_t)((buf[3]  << 8) | buf[2]);
  raw_az_ = (int16_t)((buf[5]  << 8) | buf[4]);
  raw_gx_ = (int16_t)((buf[7]  << 8) | buf[6]);
  raw_gy_ = (int16_t)((buf[9]  << 8) | buf[8]);
  raw_gz_ = (int16_t)((buf[11] << 8) | buf[10]);
  raw_ax_ = static_cast<int16_t>((buffer[1] << 8) | buffer[0]);
  raw_ay_ = static_cast<int16_t>((buffer[3] << 8) | buffer[2]);
  raw_az_ = static_cast<int16_t>((buffer[5] << 8) | buffer[4]);
  raw_gx_ = static_cast<int16_t>((buffer[7] << 8) | buffer[6]);
  raw_gy_ = static_cast<int16_t>((buffer[9] << 8) | buffer[8]);
  raw_gz_ = static_cast<int16_t>((buffer[11] << 8) | buffer[10]);

  ESP_LOGV(TAG, "RAW AX:%d AY:%d AZ:%d GX:%d GY:%d GZ:%d",
           raw_ax_, raw_ay_, raw_az_, raw_gx_, raw_gy_, raw_gz_);
  if (temperature_ != nullptr) {
    uint8_t temp_buffer[2] = {0};
    if (this->read_register(BMI270_REG_TEMP_LSB, temp_buffer, sizeof(temp_buffer))) {
      raw_temperature_ = static_cast<int16_t>((temp_buffer[1] << 8) | temp_buffer[0]);
      temperature_valid_ = true;
    } else {
      ESP_LOGW(TAG, "Impossible de lire la température BMI270");
    }
  }

  ESP_LOGV(TAG, "RAW AX:%d AY:%d AZ:%d GX:%d GY:%d GZ:%d T:%d", raw_ax_, raw_ay_, raw_az_, raw_gx_, raw_gy_, raw_gz_,
           raw_temperature_);

  return true;
}

}  // namespace sensor_bmi270
}  // namespace esphome
bool BMI270Sensor::initialise_device_() {
  vTaskDelay(pdMS_TO_TICKS(5));

  if (!this->soft_reset_()) {
    ESP_LOGE(TAG, "Échec du soft-reset BMI270");
    return false;
  }

  vTaskDelay(BMI270_RESET_DELAY);

  uint8_t chip_id = 0x00;
  bool id_ok = false;
  for (uint8_t attempt = 0; attempt < 5 && !id_ok; attempt++) {
    if (read_u8(this, BMI270_REG_CHIP_ID, chip_id) && chip_id == BMI270_CHIP_ID) {
      id_ok = true;
    } else {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }

  if (!id_ok) {
    ESP_LOGE(TAG, "Chip-ID inattendu (lu 0x%02X, attendu 0x%02X)", chip_id, BMI270_CHIP_ID);
    return false;
  }
  ESP_LOGD(TAG, "Chip-ID confirmé: 0x%02X", chip_id);

  if (!this->upload_config_file_()) {
    ESP_LOGW(TAG, "Le firmware BMI270 n'a pas pu être chargé, poursuite avec la configuration par défaut");
  }

  if (!this->set_pmu_state_()) {
    ESP_LOGE(TAG, "Impossible de configurer le PMU BMI270");
    return false;
  }

  if (!write_u8(this, BMI270_REG_ACC_CONF, 0xA8)) {
    ESP_LOGW(TAG, "Échec configuration ACC_CONF");
  }
  if (!write_u8(this, BMI270_REG_ACC_RANGE, 0x00)) {
    ESP_LOGW(TAG, "Échec configuration ACC_RANGE");
  }
  if (!write_u8(this, BMI270_REG_GYR_CONF, 0xA9)) {
    ESP_LOGW(TAG, "Échec configuration GYR_CONF");
  }
  if (!write_u8(this, BMI270_REG_GYR_RANGE, 0x00)) {
    ESP_LOGW(TAG, "Échec configuration GYR_RANGE");
  }

  uint8_t err_reg = 0x00;
  if (read_u8(this, BMI270_REG_ERR_REG, err_reg) && err_reg != 0x00) {
    ESP_LOGW(TAG, "BMI270 ERR_REG = 0x%02X", err_reg);
  }
  uint8_t status = 0x00;
  if (read_u8(this, BMI270_REG_STATUS, status)) {
    ESP_LOGD(TAG, "STATUS = 0x%02X", status);
  }
  uint8_t pmu = 0x00;
  if (read_u8(this, BMI270_REG_PMU_STATUS, pmu)) {
    ESP_LOGD(TAG, "PMU_STATUS = 0x%02X", pmu);
  }

  return true;
}

bool BMI270Sensor::soft_reset_() {
  if (!write_u8(this, BMI270_REG_CMD, BMI270_CMD_SOFTRESET)) {
    ESP_LOGW(TAG, "Soft-reset impossible sur le registre 0x%02X", BMI270_REG_CMD);
    return false;
  }
  return true;
}

bool BMI270Sensor::set_pmu_state_() {
  if (!write_u8(this, BMI270_REG_PWR_CONF, 0x00)) {
    ESP_LOGE(TAG, "Impossible d'écrire PWR_CONF");
    return false;
  }
  vTaskDelay(BMI270_POWER_ON_DELAY);

  if (!write_u8(this, BMI270_REG_PWR_CTRL, 0x0E)) {
    ESP_LOGE(TAG, "Impossible d'écrire PWR_CTRL");
    return false;
  }
  vTaskDelay(BMI270_SENSOR_STARTUP);

  return true;
}

bool BMI270Sensor::upload_config_file_() {
  if (BMI270_CONFIG_FILE_LEN == 0) {
    ESP_LOGW(TAG, "Aucun firmware BMI270 embarqué - saut du chargement INIT_DATA");
    return true;
  }

  if (!write_u8(this, BMI270_REG_INIT_CTRL, 0x00)) {
    ESP_LOGW(TAG, "Impossible de préparer INIT_CTRL pour le transfert");
    return false;
  }

  const size_t chunk_size = 16;
  for (size_t offset = 0; offset < BMI270_CONFIG_FILE_LEN; offset += chunk_size) {
    const size_t remaining = BMI270_CONFIG_FILE_LEN - offset;
    const size_t tx_len = remaining < chunk_size ? remaining : chunk_size;

    uint16_t config_page = static_cast<uint16_t>(offset / chunk_size);
    if (!write_u8(this, BMI270_REG_INIT_ADDR_LSB, static_cast<uint8_t>(config_page & 0x0F)) ||
        !write_u8(this, BMI270_REG_INIT_ADDR_MSB, static_cast<uint8_t>((config_page >> 4) & 0xFF))) {
      ESP_LOGE(TAG, "Échec de programmation de l'adresse INIT (offset %u)", static_cast<unsigned>(offset));
      return false;
    }

    if (!this->write_register(BMI270_REG_INIT_DATA, BMI270_CONFIG_FILE.data() + offset, tx_len)) {
      ESP_LOGE(TAG, "Échec de l'écriture du bloc BMI270 (offset %u)", static_cast<unsigned>(offset));
      return false;
    }
  }

  if (!write_u8(this, BMI270_REG_INIT_CTRL, 0x01)) {
    ESP_LOGW(TAG, "Impossible de finaliser le chargement INIT_CTRL");
    return false;
  }

  vTaskDelay(pdMS_TO_TICKS(2));
  uint8_t status = 0;
  if (read_u8(this, BMI270_REG_STATUS, status)) {
    ESP_LOGD(TAG, "INIT STATUS = 0x%02X", status);
  }

  return true;
}

}  // namespace sensor_bmi270
}  // namespace esphome





