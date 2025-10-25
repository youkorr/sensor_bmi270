import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome import pins

DEPENDENCIES = ['i2c']
AUTO_LOAD = []  # selon besoin

sensor_bmi270_ns = cg.esphome_ns.namespace('sensor_bmi270')
BMI270Sensor = sensor_bmi270_ns.class_('BMI270Sensor', cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(BMI270Sensor),
    cv.Optional('address', default=0x68): cv.hex_uint8_t,
    cv.Optional('update_interval', default='60s'): cv.update_interval,
    sensor.DECLARE_SENSOR('accel_x'),
    sensor.DECLARE_SENSOR('accel_y'),
    sensor.DECLARE_SENSOR('accel_z'),
    sensor.DECLARE_SENSOR('gyro_x'),
    sensor.DECLARE_SENSOR('gyro_y'),
    sensor.DECLARE_SENSOR('gyro_z'),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.I2C_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    cg.add(var.set_address(config['address']))
    cg.add(var.set_update_interval(config['update_interval']))
    i2c.setup_i2c_device(var, config)
    for name in ['accel_x','accel_y','accel_z','gyro_x','gyro_y','gyro_z']:
        sens = sensor.new_sensor(var, config[name])
        cg.add(var.set_{}(sens))  # à adapter setter pour chaque capteur
    cg.add(var.setup())
