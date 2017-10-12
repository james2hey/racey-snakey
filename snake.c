#include "system.h"
#include "pio.h"
#include "ledmat.h"

int main(void) {
    led_init();
    
    ledmat_display_column (1, 1);
}
