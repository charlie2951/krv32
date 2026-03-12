#include "seven_seg.h"
#include "delay.h"


int main(void) {
    // Initialization: Show Booting
    ss_show_word(SS_WORD_BOOT);
    delay(5000000);

    // Loop through some hex values
    for (uint16_t i = 0; i < 0x100; i++) {
        ss_write_hex(i);
        delay(2000000);
    }

    // Task Complete
    ss_show_word(SS_WORD_DONE);

    while (1); // Trap
    return 0;
}