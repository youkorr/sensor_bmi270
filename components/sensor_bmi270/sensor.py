import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from . import BMI270Sensor

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BMI270Sensor),
            # L'adresse est fournie par i2c_device_schema; pas besoin de champ custom ici
            cv.Optional("update_interval", default="60s"): cv.update_interval,
            cv.Optional("accel_x"): sensor.sensor_schema(
                unit_of_measurement="m/s²",
                accuracy_decimals=3,
                state_class="measurement",
                icon="mdi:axis-arrow",
            ),
            cv.Optional("accel_y"): sensor.sensor_schema(
                unit_of_measurement="m/s²",
                accuracy_decimals=3,
                state_class="measurement",
                icon="mdi:axis-arrow",
            ),
            cv.Optional("accel_z"): sensor.sensor_schema(
                unit_of_measurement="m/s²",
                accuracy_decimals=3,
                state_class="measurement",
                icon="mdi:axis-arrow",
            ),
            cv.Optional("gyro_x"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                state_class="measurement",
                icon="mdi:rotate-3d-variant",
            ),
            cv.Optional("gyro_y"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                state_class="measurement",
                icon="mdi:rotate-3d-variant",
            ),
            cv.Optional("gyro_z"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                state_class="measurement",
                icon="mdi:rotate-3d-variant",
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x68))  # 0x68 par défaut; pourra être 0x69 au YAML
)

async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # Ne PAS gérer l'adresse ici : c'est déjà fait par register_i2c_device

    for name in ["accel_x", "accel_y", "accel_z", "gyro_x", "gyro_y", "gyro_z"]:
        if name in config:
            s = await sensor.new_sensor(config[name])
            cg.add(getattr(var, f"set_{name}_sensor")(s))


