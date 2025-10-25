import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]

sensor_bmi270_ns = cg.esphome_ns.namespace("sensor_bmi270")
BMI270Sensor = sensor_bmi270_ns.class_(
    "BMI270Sensor", cg.PollingComponent, i2c.I2CDevice
)

# Définition du schéma YAML
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BMI270Sensor),
            cv.Optional("address", default=0x68): cv.hex_uint8_t,
            cv.Optional("update_interval", default="60s"): cv.update_interval,
            cv.Optional("accel_x"): sensor.sensor_schema(
                unit_of_measurement="g",
                accuracy_decimals=3,
                device_class="acceleration",
                state_class="measurement",
            ),
            cv.Optional("accel_y"): sensor.sensor_schema(
                unit_of_measurement="g",
                accuracy_decimals=3,
                device_class="acceleration",
                state_class="measurement",
            ),
            cv.Optional("accel_z"): sensor.sensor_schema(
                unit_of_measurement="g",
                accuracy_decimals=3,
                device_class="acceleration",
                state_class="measurement",
            ),
            cv.Optional("gyro_x"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                device_class="angular_velocity",
                state_class="measurement",
            ),
            cv.Optional("gyro_y"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                device_class="angular_velocity",
                state_class="measurement",
            ),
            cv.Optional("gyro_z"): sensor.sensor_schema(
                unit_of_measurement="°/s",
                accuracy_decimals=2,
                device_class="angular_velocity",
                state_class="measurement",
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x68))
)


async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if "address" in config:
        cg.add(var.set_address(config["address"]))

    # Associer les sous-capteurs
    for name in ["accel_x", "accel_y", "accel_z", "gyro_x", "gyro_y", "gyro_z"]:
        if name in config:
            sens = await sensor.new_sensor(config[name])
            cg.add(getattr(var, f"set_{name}_sensor")(sens))

