/*
    MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Driver for DHT11 temperature and Humidity
Version-1.0
Author: Subir Maity
*/

#include "dht11.h"

// Initialize DHT11 sensor
void dht11_init(uint8_t pin)
{
    dht_pin = pin;
    pinMode(dht_pin, OUTPUT);
    digitalWrite(dht_pin, HIGH);
}


/* -------------------------------------------------
 * Low-level communication
 * ------------------------------------------------- */

void dht11_start_signal(void)
{  //uart_sendline(UART1, "DHT sensor initialized \r\n");
    pinMode(dht_pin, OUTPUT);
    digitalWrite(dht_pin, LOW);
    timer_delay_ms(18);//18ms
    digitalWrite(dht_pin, HIGH);
    timer_delay_us(40);//40 micros
    pinMode(dht_pin, INPUT);
}

static char dht11_read_byte(void)
{
    uint8_t value = 0;

    for (int i = 0; i < 8; i++)
    {
        while (digitalRead(dht_pin) == LOW);
            // uart_sendline(UART1, "DHT pin still low...waiting \r\n");
             
        timer_delay_us(25); // this must be accurate for proper sampling 

        if (digitalRead(dht_pin) == HIGH)
            value |= (1 << (7 - i));

        while (digitalRead(dht_pin) == HIGH)
            ;
    }
    return value;
}

 int dht11_read_raw(char data[5])
{
    //timer_delay_ms(1000);
    dht11_start_signal();

    static uint32_t TIMEOUT=1000;
    volatile uint32_t count=0;

    while (digitalRead(dht_pin) == HIGH)
{
    if (++count > TIMEOUT)
        return DHT11_ERROR_TIMEOUT;
}

    if (digitalRead(dht_pin) == LOW)
    {
        //uart_sendline(UART1, "DHT pin goes low \r\n");
        timer_delay_us(80);//check this

        if (digitalRead(dht_pin) == HIGH)
        {
              //uart_sendline(UART1, "DHT pin goes high \r\n");
            timer_delay_us(80); //check this

            for (int i = 0; i < 5; i++)
            {
                data[i] = dht11_read_byte();
            }

            if (data[4] ==
                ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 2;
}

/* -------------------------------------------------
 * Public read functions
 * ------------------------------------------------- */

 int dht11_read_temperature(void)
{
    static char data[5];
    int ret = dht11_read_raw(data);
    if (ret != 0)
        return ret;
    return data[2];
}

 int dht11_read_humidity(void)
{
    static char data[5];
    int ret = dht11_read_raw(data);
    if (ret != 0)
        return ret;
    return data[0];
}

 int dht11_read_temperature_humidity(uint32_t *temperature, uint32_t *humidity)
{
    static char data[5];
    int ret = dht11_read_raw(data);
    if (ret != 0)
        return ret;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}


