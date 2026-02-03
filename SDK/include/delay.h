
#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 * @brief Simple busy-wait delay loop
 * @param cycles Number of iterations to wait
 */
void delay(uint32_t cycles);

#endif