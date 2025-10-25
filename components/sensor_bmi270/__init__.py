import esphome.codegen as cg
from esphome.components import i2c

sensor_bmi270_ns = cg.esphome_ns.namespace("sensor_bmi270")
BMI270Sensor = sensor_bmi270_ns.class_("BMI270Sensor", cg.PollingComponent, i2c.I2CDevice)



