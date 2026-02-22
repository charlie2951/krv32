// Sample demo programme to interface SSD1306 OLED
#include "i2c.h"
#include "ssd1306.h"

#define I2C I2C0 //using I2C-0
int main(void)
{
    // Initialize I2C0 for OLED
   // i2c_init(&I2C, 499);

    ssd1306_init(&I2C);

    ssd1306_set_cursor(0, 0);
    ssd1306_write_string("Hello FPGA");

    while (1);
}
