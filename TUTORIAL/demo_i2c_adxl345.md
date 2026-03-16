# Interfacing ADXL345 using hardware I2C
Connect `CS` line of sensor to 3.3V, SCL to SCL line of I2C port, SDA to SDA port of I2C, SD0 should be grounded.<p>
For **`I2C0`**, use: <p>
JA[3]-> SDA<p></p>
JA[4]-> SCL<p>
For **`I2C1`** , use: <p>
JA[5]-> SDA<p>
JA[6]-> SCL<p>
Also, change the `I2C` instance in software code to change i2c port.<p>
Upload the firmware and see acceleration data on serial monitor or run `python3 read_serial.py COM_NO` to see data on command prompt. Put proper COM port number of your machine.

