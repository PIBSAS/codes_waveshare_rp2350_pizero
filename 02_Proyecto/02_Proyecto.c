#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);

    while (1) {
        printf("Flash size configurado: %u bytes\n", PICO_FLASH_SIZE_BYTES); // Ver dimension de la flash
        sleep_ms(1000);
    }
}