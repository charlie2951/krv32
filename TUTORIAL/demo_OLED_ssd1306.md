### SSD1306-based OLED Interfacing demo.
Use PMOD JB pin1 for display `SDA` and pin2 for `SCL` line of SSD1306 I2C OLED display.<p>
Sample program:<p>
```
#include "softi2c.h"
#include "ssd1306.h"
int main(void)
{
    softi2c_init();//initialize software-i2c master
    ssd1306_init();//initialize display

    ssd1306_set_cursor(0, 4);//set cursor position, see specs of ssd1306
    ssd1306_write_string("KRV-32 Soft SoC!");
    ssd1306_set_cursor(4, 4);
    ssd1306_write_string("@@ HELLO RISC-V @@");

    while (1);
}
```
Upload the code and wait a while, the string will be displayed on the module. Initially, the whole display will be cleared, and it
will take some time. Then the desired character will be displayed.
