```
external_components:
  - source: local
    path: components/sensor_bmi270

i2c:
  sda: 21
  scl: 22
  scan: true

sensor:
  - platform: sensor_bmi270
    id: bmi270_1
    address: 0x68
    update_interval: 30s
    accel_x:
      name: "BMI270 Accel X"
      unit_of_measurement: "g"
      device_class: acceleration
      state_class: measurement
    accel_y:
      name: "BMI270 Accel Y"
      unit_of_measurement: "g"
      device_class: acceleration
      state_class: measurement
    accel_z:
      name: "BMI270 Accel Z"
      unit_of_measurement: "g"
      device_class: acceleration
      state_class: measurement
    gyro_x:
      name: "BMI270 Gyro X"
      unit_of_measurement: "°/s"
      device_class: angular_velocity
      state_class: measurement
    gyro_y:
      name: "BMI270 Gyro Y"
      unit_of_measurement: "°/s"
      device_class: angular_velocity
      state_class: measurement
    gyro_z:
      name: "BMI270 Gyro Z"
      unit_of_measurement: "°/s"
      device_class: angular_velocity
      state_class: measurement

      ```

