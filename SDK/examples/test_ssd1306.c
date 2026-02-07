#include "softi2c.h"
#include "ssd1306.h"
int main(void)
{
    softi2c_init();
    ssd1306_init();

    ssd1306_set_cursor(0, 4);
    ssd1306_write_string("KRV-32 Soft SoC!");
    ssd1306_set_cursor(4, 4);
    ssd1306_write_string("@@ HELLO RISC-V @@");

    while (1);
}
