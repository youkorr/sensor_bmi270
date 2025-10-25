#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace esphome {
namespace sensor_bmi270 {

// Contenu binaire du firmware BMI270 (à charger via upload_config_file_()).
// La distribution originale fournie par Bosch via M5Stack n'est pas redistribuable.
// Par défaut, nous utilisons un tableau vide ce qui laisse le BMI270 fonctionner
// avec sa configuration ROM minimaliste.
constexpr std::array<uint8_t, 0> BMI270_CONFIG_FILE{};
constexpr size_t BMI270_CONFIG_FILE_LEN = BMI270_CONFIG_FILE.size();

}  // namespace sensor_bmi270
}  // namespace esphome
